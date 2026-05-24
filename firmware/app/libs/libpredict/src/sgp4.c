#include <predict/defs.h>
#include <predict/sgp4.h>
#include <predict/unsorted.h>

void sgp4_init( const predict_orbital_elements_t * orbital_elements,
                struct predict_sgp4 * m )
{
    m->simpleFlag = 0;

    // Calculate old TLE field values as used in the original sgp4
    double temp_tle = TWO_PI / MINUTES_PER_DAY / MINUTES_PER_DAY;
    m->bstar = orbital_elements->bstar_drag_term / AE;
    m->xincl = orbital_elements->inclination * M_PI / 180.0;
    m->xnodeo = orbital_elements->right_ascension * M_PI / 180.0;
    m->eo = orbital_elements->eccentricity;
    m->omegao = orbital_elements->argument_of_perigee * M_PI / 180.0;
    m->xmo = orbital_elements->mean_anomaly * M_PI / 180.0;
    m->xno = orbital_elements->mean_motion * temp_tle * MINUTES_PER_DAY;

    double x1m5th;
    double xhdot1;
    double a1;
    double a3ovk2;
    double ao;
    double betao;
    double betao2;
    double c1sq;
    double c2;
    double c3;
    double coef;
    double coef1;
    double del1;
    double delo;
    double eeta;
    double eosq;
    double etasq;
    double perigee;
    double pinvsq;
    double psisq;
    double qoms24;
    double s4;
    double temp;
    double temp1;
    double temp2;
    double temp3;
    double theta2;
    double theta4;
    double tsi;

    /* Recover original mean motion (m->xnodp) and   */
    /* semimajor axis (m->aodp) from input elements. */

    a1 = pow( XKE / m->xno, TWO_THIRD );
    m->cosio = cos( m->xincl );
    theta2 = m->cosio * m->cosio;
    m->x3thm1 = ( 3.0 * theta2 ) - 1.0;
    eosq = m->eo * m->eo;
    betao2 = 1.0 - eosq;
    betao = sqrt( betao2 );
    del1 = 1.5 * CK2 * m->x3thm1 / ( a1 * a1 * betao * betao2 );
    ao = a1 *
         ( 1.0 - ( del1 * ( ( 0.5 * TWO_THIRD ) +
                            ( del1 * ( 1.0 + ( 134.0 / 81.0 * del1 ) ) ) ) ) );
    delo = 1.5 * CK2 * m->x3thm1 / ( ao * ao * betao * betao2 );
    m->xnodp = m->xno / ( 1.0 + delo );
    m->aodp = ao / ( 1.0 - delo );

    /* For perigee less than 220 kilometers, the "simple"     */
    /* flag is set and the equations are truncated to linear  */
    /* variation in sqrt a and quadratic variation in mean    */
    /* anomaly.  Also, the c3 term, the delta omega term, and */
    /* the delta m term are dropped.                          */

    if( ( m->aodp * ( 1.0 - m->eo ) / AE ) <
        ( ( 220.0 / EARTH_RADIUS_KM_WGS84 ) + AE ) )
    {
        m->simpleFlag = true;
    }
    else
    {
        m->simpleFlag = false;
    }

    /* For perigees below 156 km, the      */
    /* values of s and qoms2t are altered. */

    s4 = S_DENSITY_PARAM;
    qoms24 = QOMS2T;
    perigee = ( m->aodp * ( 1 - m->eo ) - AE ) * EARTH_RADIUS_KM_WGS84;

    if( perigee < 156.0 )
    {
        if( perigee <= 98.0 )
        {
            s4 = 20.0;
        }
        else
        {
            s4 = perigee - 78.0;
        }

        qoms24 = pow( ( 120.0 - s4 ) * AE / EARTH_RADIUS_KM_WGS84, 4.0 );
        s4 = ( s4 / EARTH_RADIUS_KM_WGS84 ) + AE;
    }

    pinvsq = 1 / ( m->aodp * m->aodp * betao2 * betao2 );
    tsi = 1 / ( m->aodp - s4 );
    m->eta = m->aodp * m->eo * tsi;
    etasq = m->eta * m->eta;
    eeta = m->eo * m->eta;
    psisq = fabs( 1.0 - etasq );
    coef = qoms24 * pow( tsi, 4 );
    coef1 = coef / pow( psisq, 3.5 );
    c2 = coef1 * m->xnodp *
         ( ( m->aodp *
             ( 1.0 + ( 1.5 * etasq ) + ( eeta * ( 4.0 + etasq ) ) ) ) +
           ( 0.75 * CK2 * tsi / psisq * m->x3thm1 *
             ( 8.0 + ( 3.0 * etasq * ( 8.0 + etasq ) ) ) ) );
    m->c1 = m->bstar * c2;
    m->sinio = sin( m->xincl );
    a3ovk2 = -J3_HARMONIC_WGS72 / CK2 * pow( AE, 3 );
    c3 = coef * tsi * a3ovk2 * m->xnodp * AE * m->sinio / m->eo;
    m->x1mth2 = 1.0 - theta2;

    m->c4 = 2.0 * m->xnodp * coef1 * m->aodp * betao2 *
            ( ( m->eta * ( 2.0 + ( 0.5 * etasq ) ) ) +
              ( m->eo * ( 0.5 + ( 2.0 * etasq ) ) ) -
              ( 2.0 * CK2 * tsi / ( m->aodp * psisq ) ) *
                  ( -3.0 * m->x3thm1 *
                        ( 1.0 - ( 2.0 * eeta ) +
                          ( etasq * ( 1.5 - ( 0.5 * eeta ) ) ) ) +
                    ( 0.75 * m->x1mth2 *
                      ( ( 2.0 * etasq ) - ( eeta * ( 1.0 + etasq ) ) ) *
                      cos( 2.0 * m->omegao ) ) ) );
    m->c5 = 2.0 * coef1 * m->aodp * betao2 *
            ( 1.0 + ( 2.75 * ( etasq + eeta ) ) + ( eeta * etasq ) );

    theta4 = theta2 * theta2;
    temp1 = 3.0 * CK2 * pinvsq * m->xnodp;
    temp2 = temp1 * CK2 * pinvsq;
    temp3 = 1.25 * CK4 * pinvsq * pinvsq * m->xnodp;
    m->xmdot = m->xnodp + ( 0.5 * temp1 * betao * m->x3thm1 ) +
               ( 0.0625 * temp2 * betao *
                 ( 13.0 - ( 78.0 * theta2 ) + ( 137.0 * theta4 ) ) );
    x1m5th = 1.0 - ( 5.0 * theta2 );
    m->omgdot = ( -0.5 * temp1 * x1m5th ) +
                ( 0.0625 * temp2 *
                  ( 7.0 - ( 114.0 * theta2 ) + ( 395.0 * theta4 ) ) ) +
                ( temp3 * ( 3.0 - ( 36.0 * theta2 ) + ( 49.0 * theta4 ) ) );
    xhdot1 = -temp1 * m->cosio;
    m->xnodot = xhdot1 + ( ( 0.5 * temp2 * ( 4.0 - ( 19.0 * theta2 ) ) +
                             ( 2.0 * temp3 * ( 3.0 - ( 7.0 * theta2 ) ) ) ) *
                           m->cosio );
    m->omgcof = m->bstar * c3 * cos( m->omegao );
    m->xmcof = -TWO_THIRD * coef * m->bstar * AE / eeta;
    m->xnodcf = 3.5 * betao2 * xhdot1 * m->c1;
    m->t2cof = 1.5 * m->c1;
    m->xlcof = 0.125 * a3ovk2 * m->sinio * ( 3.0 + ( 5.0 * m->cosio ) ) /
               ( 1.0 + m->cosio );
    m->aycof = 0.25 * a3ovk2 * m->sinio;
    m->delmo = pow( 1.0 + ( m->eta * cos( m->xmo ) ), 3 );
    m->sinmo = sin( m->xmo );
    m->x7thm1 = ( 7.0 * theta2 ) - 1.0;

    if( !m->simpleFlag )
    {
        c1sq = m->c1 * m->c1;
        m->d2 = 4.0 * m->aodp * tsi * c1sq;
        temp = m->d2 * tsi * m->c1 / 3.0;
        m->d3 = ( ( 17.0 * m->aodp ) + s4 ) * temp;
        m->d4 = 0.5 * temp * m->aodp * tsi *
                ( ( 221.0 * m->aodp ) + ( 31.0 * s4 ) ) * m->c1;
        m->t3cof = m->d2 + ( 2.0 * c1sq );
        m->t4cof = 0.25 * ( ( 3.0 * m->d3 ) + ( m->c1 * ( ( 12.0 * m->d2 ) +
                                                          ( 10.0 * c1sq ) ) ) );
        m->t5cof = 0.2 * ( ( 3.0 * m->d4 ) + ( 12.0 * m->c1 * m->d3 ) +
                           ( 6.0 * m->d2 * m->d2 ) +
                           ( 15.0 * c1sq * ( ( 2.0 * m->d2 ) + c1sq ) ) );
    }
}

void sgp4_predict( const struct predict_sgp4 * m,
                   double tsince,
                   struct model_output * output )
{
    double cosuk;
    double sinuk;
    double rfdotk;
    double vx;
    double vy;
    double vz;
    double ux;
    double uy;
    double uz;
    double xmy;
    double xmx;
    double cosnok;
    double sinnok;
    double cosik;
    double sinik;
    double rdotk;
    double xinck;
    double xnodek;
    double uk;
    double rk;
    double cos2u;
    double sin2u;
    double u;
    double sinu;
    double cosu;
    double betal;
    double rfdot;
    double rdot;
    double r;
    double pl;
    double elsq;
    double esine;
    double ecose;
    double epw;
    double cosepw;
    double tfour;
    double sinepw;
    double capu;
    double ayn;
    double xlt;
    double aynl;
    double xll;
    double axn;
    double xn;
    double beta;
    double xl;
    double e;
    double a;
    double tcube;
    double delm;
    double delomg;
    double templ;
    double tempe;
    double tempa;
    double xnode;
    double tsq;
    double xmp;
    double omega;
    double xnoddf;
    double omgadf;
    double xmdf;
    double temp;
    double temp1;
    double temp2;
    double temp3;
    double temp4;
    double temp5;
    double temp6;

    int32_t i;

    /* Update for secular gravity and atmospheric drag. */
    xmdf = m->xmo + ( m->xmdot * tsince );
    omgadf = m->omegao + ( m->omgdot * tsince );
    xnoddf = m->xnodeo + ( m->xnodot * tsince );
    omega = omgadf;
    xmp = xmdf;
    tsq = tsince * tsince;
    xnode = xnoddf + ( m->xnodcf * tsq );
    tempa = 1 - ( m->c1 * tsince );
    tempe = m->bstar * m->c4 * tsince;
    templ = m->t2cof * tsq;

    if( !m->simpleFlag )
    {
        delomg = m->omgcof * tsince;
        delm = m->xmcof * ( pow( 1 + ( m->eta * cos( xmdf ) ), 3 ) - m->delmo );
        temp = delomg + delm;
        xmp = xmdf + temp;
        omega = omgadf - temp;
        tcube = tsq * tsince;
        tfour = tsince * tcube;
        tempa = tempa - ( m->d2 * tsq ) - ( m->d3 * tcube ) - ( m->d4 * tfour );
        tempe = tempe + ( m->bstar * m->c5 * ( sin( xmp ) - m->sinmo ) );
        templ = templ + ( m->t3cof * tcube ) +
                ( tfour * ( m->t4cof + ( tsince * m->t5cof ) ) );
    }

    a = m->aodp * pow( tempa, 2 );
    e = m->eo - tempe;
    xl = xmp + omega + xnode + ( m->xnodp * templ );
    beta = sqrt( 1.0 - ( e * e ) );
    xn = XKE / pow( a, 1.5 );

    /* Long period periodics */
    axn = e * cos( omega );
    temp = 1.0 / ( a * beta * beta );
    xll = temp * m->xlcof * axn;
    aynl = temp * m->aycof;
    xlt = xl + xll;
    ayn = e * sin( omega ) + aynl;

    /* Solve Kepler's Equation */
    capu = FMod2p( xlt - xnode );
    temp2 = capu;
    i = 0;

    do
    {
        sinepw = sin( temp2 );
        cosepw = cos( temp2 );
        temp3 = axn * sinepw;
        temp4 = ayn * cosepw;
        temp5 = axn * cosepw;
        temp6 = ayn * sinepw;
        epw = ( capu - temp4 + temp3 - temp2 ) / ( 1.0 - temp5 - temp6 ) +
              temp2;

        if( fabs( epw - temp2 ) <= E6A )
        {
            break;
        }

        temp2 = epw;

    } while( i++ < 10 );

    /* Short period preliminary quantities */
    ecose = temp5 + temp6;
    esine = temp3 - temp4;
    elsq = ( axn * axn ) + ( ayn * ayn );
    temp = 1.0 - elsq;
    pl = a * temp;
    r = a * ( 1.0 - ecose );
    temp1 = 1.0 / r;
    rdot = XKE * sqrt( a ) * esine * temp1;
    rfdot = XKE * sqrt( pl ) * temp1;
    temp2 = a * temp1;
    betal = sqrt( temp );
    temp3 = 1.0 / ( 1.0 + betal );
    cosu = temp2 * ( cosepw - axn + ( ayn * esine * temp3 ) );
    sinu = temp2 * ( sinepw - ayn - ( axn * esine * temp3 ) );
    u = atan2( sinu, cosu );
    sin2u = 2.0 * sinu * cosu;
    cos2u = ( 2.0 * cosu * cosu ) - 1.0;
    temp = 1.0 / pl;
    temp1 = CK2 * temp;
    temp2 = temp1 * temp;

    /* Update for short periodics */
    rk = ( r * ( 1.0 - ( 1.5 * temp2 * betal * m->x3thm1 ) ) ) +
         ( 0.5 * temp1 * m->x1mth2 * cos2u );
    uk = u - ( 0.25 * temp2 * m->x7thm1 * sin2u );
    xnodek = xnode + ( 1.5 * temp2 * m->cosio * sin2u );
    xinck = m->xincl + ( 1.5 * temp2 * m->cosio * m->sinio * cos2u );
    rdotk = rdot - ( xn * temp1 * m->x1mth2 * sin2u );
    rfdotk = rfdot +
             ( xn * temp1 * ( ( m->x1mth2 * cos2u ) + ( 1.5 * m->x3thm1 ) ) );

    /* Orientation vectors */
    sinuk = sin( uk );
    cosuk = cos( uk );
    sinik = sin( xinck );
    cosik = cos( xinck );
    sinnok = sin( xnodek );
    cosnok = cos( xnodek );
    xmx = -sinnok * cosik;
    xmy = cosnok * cosik;
    ux = ( xmx * sinuk ) + ( cosnok * cosuk );
    uy = ( xmy * sinuk ) + ( sinnok * cosuk );
    uz = sinik * sinuk;
    vx = ( xmx * cosuk ) - ( cosnok * sinuk );
    vy = ( xmy * cosuk ) - ( sinnok * sinuk );
    vz = sinik * cosuk;

    /* Position and velocity */
    output->pos[ 0 ] = rk * ux;
    output->pos[ 1 ] = rk * uy;
    output->pos[ 2 ] = rk * uz;
    output->vel[ 0 ] = ( rdotk * ux ) + ( rfdotk * vx );
    output->vel[ 1 ] = ( rdotk * uy ) + ( rfdotk * vy );
    output->vel[ 2 ] = ( rdotk * uz ) + ( rfdotk * vz );

    /* Phase in radians */
    output->phase = xlt - xnode - omgadf + TWO_PI;

    if( output->phase < 0.0 )
    {
        output->phase += TWO_PI;
    }

    output->phase = FMod2p( output->phase );

    output->xinck = xinck;
    output->omgadf = omgadf;
    output->xnodek = xnodek;
}
