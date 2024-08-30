#include <stddef.h>
#include <stdint.h>

#include <predict/defs.h>
#include <predict/unsorted.h>

void vec3_set( double v[ 3 ], double x, double y, double z )
{
    v[ 0 ] = x;
    v[ 1 ] = y;
    v[ 2 ] = z;
}

double vec3_length( const double v[ 3 ] )
{
    return sqrt( ( v[ 0 ] * v[ 0 ] ) + ( v[ 1 ] * v[ 1 ] ) +
                 ( v[ 2 ] * v[ 2 ] ) );
}

double vec3_dot( const double v[ 3 ], const double u[ 3 ] )
{
    return ( ( v[ 0 ] * u[ 0 ] ) + ( v[ 1 ] * u[ 1 ] ) + ( v[ 2 ] * u[ 2 ] ) );
}

void vec3_mul_scalar( const double v[ 3 ], double a, double r[ 3 ] )
{
    r[ 0 ] = v[ 0 ] * a;
    r[ 1 ] = v[ 1 ] * a;
    r[ 2 ] = v[ 2 ] * a;
}

void vec3_sub( const double v1[ 3 ], const double v2[ 3 ], double * r )
{
    r[ 0 ] = v1[ 0 ] - v2[ 0 ];
    r[ 1 ] = v1[ 1 ] - v2[ 1 ];
    r[ 2 ] = v1[ 2 ] - v2[ 2 ];
}

double Sqr( double arg )
{
    /* Returns square of a double */
    return ( arg * arg );
}

double FMod2p( double x )
{
    /* Returns mod 2PI of argument */
    double ret_val = fmod( x, ( double ) 2 * ( double ) M_PI );

    if( ret_val < 0.0 )
    {
        ret_val += ( ( double ) 2 * ( double ) M_PI );
    }

    return ret_val;
}

void Convert_Sat_State( double pos[ 3 ], double vel[ 3 ] )
{
    /* Converts the satellite's position and velocity  */
    /* vectors from normalized values to km and km/sec */

    vec3_mul_scalar( pos, EARTH_RADIUS_KM_WGS84, pos );
    vec3_mul_scalar( vel,
                     EARTH_RADIUS_KM_WGS84 * MINUTES_PER_DAY / SECONDS_PER_DAY,
                     vel );
}

double Julian_Date_of_Year( double year )
{
    /* The function Julian_Date_of_Year calculates the Julian Date  */
    /* of Day 0.0 of {year}. This function is used to calculate the */
    /* Julian Date of any date by using Julian_Date_of_Year, DOY,   */
    /* and Fraction_of_Day. */

    /* Astronomical Formulae for Calculators, Jean Meeus, */
    /* pages 23-25. Calculate Julian Date of 0.0 Jan year */

    double y = year;
    double jdoy = ( 30.6001 * 14.0 );

    int64_t A;
    int64_t B;
    int64_t i;

    y--;
    i = ( int64_t ) y / 100.0;
    A = i;
    i = A / ( int64_t ) 4;
    B = ( int64_t ) 2 - A + i;
    i = ( int64_t ) ( 365.25 * y );
    i += ( int64_t ) jdoy;
    jdoy = ( double ) i + ( double ) 1720994.5 + ( double ) B;

    return jdoy;
}

double Julian_Date_of_Epoch( double epoch )
{
    /* The function Julian_Date_of_Epoch returns the Julian Date of     */
    /* an epoch specified in the format used in the NORAD two-line      */
    /* element sets. It has been modified to support dates beyond       */
    /* the year 1999 assuming that two-digit years in the range 00-56   */
    /* correspond to 2000-2056. Until the two-line element set format   */
    /* is changed, it is only valid for dates through 2056 December 31. */

    double year;
    double day;

    /* Modification to support Y2K */
    /* Valid 1957 through 2056     */

    day = modf( epoch * 1E-3, &year ) * 1E3;

    if( year < 57.0 )
    {
        year += ( double ) 2000.0;
    }
    else
    {
        year += ( double ) 1900.0;
    }

    return ( Julian_Date_of_Year( year ) + day );
}

double ThetaG_JD( double jd )
{
    /* Reference:  The 1992 Astronomical Almanac, page B6. */

    double UT;
    double TU;
    double GMST;
    double j = jd;

    double dummy;
    UT = modf( j + 0.5, &dummy );
    j -= UT;
    TU = ( j - 2451545.0 ) / 36525.0;
    GMST = 24110.54841 + ( TU * ( 8640184.812866 +
                                  ( TU * ( 0.093104 - ( TU * 6.2E-6 ) ) ) ) );
    GMST = fmod( GMST + ( SECONDS_PER_DAY * EARTH_ROTATIONS_PER_SIDERIAL_DAY *
                          UT ),
                 SECONDS_PER_DAY );

    return ( 2 * M_PI * GMST / SECONDS_PER_DAY );
}

void Calculate_User_PosVel( double time,
                            geodetic_t * geodetic,
                            double obs_pos[ 3 ],
                            double obs_vel[ 3 ] )
{
    /* Calculate_User_PosVel() passes the user's geodetic position
       and the time of interest and returns the ECI position and
       velocity of the observer.  The velocity calculation assumes
       the geodetic position is stationary relative to the earth's
       surface. */

    /* Reference:  The 1992 Astronomical Almanac, page K11. */

    double c;
    double sq;
    double achcp;

    geodetic->theta = FMod2p( ThetaG_JD( time ) + geodetic->lon ); /* LMST */
    c = 1 / sqrt( 1 + FLATTENING_FACTOR * ( FLATTENING_FACTOR - 2 ) *
                          Sqr( sin( geodetic->lat ) ) );
    sq = Sqr( 1 - FLATTENING_FACTOR ) * c;
    achcp = ( ( EARTH_RADIUS_KM_WGS84 * c ) + geodetic->alt ) *
            cos( geodetic->lat );
    obs_pos[ 0 ] = ( achcp * cos( geodetic->theta ) ); /* kilometers */
    obs_pos[ 1 ] = ( achcp * sin( geodetic->theta ) );
    obs_pos[ 2 ] = ( ( ( EARTH_RADIUS_KM_WGS84 * sq ) + geodetic->alt ) *
                     sin( geodetic->lat ) );
    obs_vel[ 0 ] = ( -EARTH_ANGULAR_VELOCITY *
                     obs_pos[ 1 ] ); /* kilometers/second
                                      */
    obs_vel[ 1 ] = ( EARTH_ANGULAR_VELOCITY * obs_pos[ 0 ] );
    obs_vel[ 2 ] = ( 0 );
}

int64_t DayNum( int32_t m, int32_t d, int32_t y )
{
    int64_t dn;
    double mm;
    double yy;

    int32_t year = y;
    int32_t month = m;

    if( month < 3 )
    {
        year -= 1;
        month += 12;
    }

    if( year < 57 )
    {
        year += 100;
    }

    yy = ( double ) year;
    mm = ( double ) month;
    dn = ( floor( 365.25 * ( yy - 80.0 ) ) - floor( 19.0 + ( yy / 100.0 ) ) +
           floor( 4.75 + ( yy / 400.0 ) ) - 16.0 );
    dn += ( int64_t ) d + ( 30 * ( int64_t ) m ) +
          ( int64_t ) floor( ( 0.6 * mm ) - 0.3 );
    return dn;
}

void Calculate_LatLonAlt( double time,
                          const double pos[ 3 ],
                          geodetic_t * geodetic )
{
    /* Procedure Calculate_LatLonAlt will calculate the geodetic  */
    /* position of an object given its ECI position pos and time. */
    /* It is intended to be used to determine the ground track of */
    /* a satellite.  The calculations  assume the earth to be an  */
    /* oblate spheroid as defined in WGS '72.                     */

    /* Reference:  The 1992 Astronomical Almanac, page K12. */

    double r;
    double e2;
    double phi;
    double c;

    double t = time;

    geodetic->theta = atan2( pos[ 1 ], pos[ 0 ] );              /* radians */
    geodetic->lon = FMod2p( geodetic->theta - ThetaG_JD( t ) ); /* radians */
    r = sqrt( Sqr( pos[ 0 ] ) + Sqr( pos[ 1 ] ) );
    e2 = FLATTENING_FACTOR * ( 2.0 - FLATTENING_FACTOR );
    geodetic->lat = atan2( pos[ 2 ], r ); /* radians */

    do
    {
        phi = geodetic->lat;
        c = 1.0 / sqrt( 1.0 - ( e2 * Sqr( sin( phi ) ) ) );
        geodetic->lat = atan2( pos[ 2 ] + ( EARTH_RADIUS_KM_WGS84 * c * e2 *
                                            sin( phi ) ),
                               r );

    } while( fabs( geodetic->lat - phi ) >= 1E-10 );

    geodetic->alt = r / cos( geodetic->lat ) -
                    ( EARTH_RADIUS_KM_WGS84 * c ); /* kilometers */

    predictFIX_ANGLE( geodetic->lat, PI_HALF );
    predictFIX_ANGLE( geodetic->lon, M_PI );
}

void Calculate_Obs( double time,
                    const double pos[ 3 ],
                    const double vel[ 3 ],
                    geodetic_t * geodetic,
                    vector_t * obs_set )
{
    /* The procedures Calculate_Obs and Calculate_RADec calculate         */
    /* the *topocentric* coordinates of the object with ECI position,     */
    /* {pos}, and velocity, {vel}, from location {geodetic} at {time}.    */
    /* The {obs_set} returned for Calculate_Obs consists of azimuth,      */
    /* elevation, range, and range rate (in that order) with units of     */
    /* radians, radians, kilometers, and kilometers/second, respectively. */
    /* The WGS '72 geoid is used and the effect of atmospheric refraction */
    /* (under standard temperature and pressure) is incorporated into the */
    /* elevation calculation; the effect of atmospheric refraction on     */
    /* range and range rate has not yet been quantified.                  */

    /* The {obs_set} for Calculate_RADec consists of right ascension and  */
    /* declination (in that order) in radians.  Again, calculations are   */
    /* based on *topocentric* position using the WGS '72 geoid and        */
    /* incorporating atmospheric refraction.                              */

    double sin_lat;
    double cos_lat;
    double sin_theta;
    double cos_theta;
    double el;
    double azim;
    double top_s;
    double top_e;
    double top_z;

    double obs_pos[ 3 ];
    double obs_vel[ 3 ];
    double range[ 3 ];
    double rgvel[ 3 ];

    Calculate_User_PosVel( time, geodetic, obs_pos, obs_vel );

    vec3_sub( pos, obs_pos, range );
    vec3_sub( vel, obs_vel, rgvel );

    double range_length = vec3_length( range );

    sin_lat = sin( geodetic->lat );
    cos_lat = cos( geodetic->lat );
    sin_theta = sin( geodetic->theta );
    cos_theta = cos( geodetic->theta );
    top_s = ( sin_lat * cos_theta * range[ 0 ] ) +
            ( sin_lat * sin_theta * range[ 1 ] ) - ( cos_lat * range[ 2 ] );
    top_e = ( -sin_theta * range[ 0 ] ) + ( cos_theta * range[ 1 ] );
    top_z = ( cos_lat * cos_theta * range[ 0 ] ) +
            ( cos_lat * sin_theta * range[ 1 ] ) + ( sin_lat * range[ 2 ] );
    azim = atan( -top_e / top_s ); /* Azimuth */

    if( top_s > 0.0 )
    {
        azim = azim + PI;
    }

    if( azim < 0.0 )
    {
        azim = azim + ( 2.0 * M_PI );
    }

    el = asin_( top_z / range_length );
    obs_set->x = azim;         /* Azimuth (radians)   */
    obs_set->y = el;           /* Elevation (radians) */
    obs_set->z = range_length; /* Range (kilometers)  */

    /* Range Rate (kilometers/second) */
    obs_set->w = vec3_dot( range, rgvel ) / vec3_length( range );
    obs_set->y = el;

    /**** End bypass ****/

    if( obs_set->y < 0.0 )
    {
        obs_set->y = el; /* Reset to true elevation */
    }
}

void Calculate_RADec( double time,
                      const double pos[ 3 ],
                      const double vel[ 3 ],
                      geodetic_t * geodetic,
                      vector_t * obs_set )
{
    /* Reference:  Methods of Orbit Determination by  */
    /*             Pedro Ramon Escobal, pp. 401-402   */

    double phi;
    double theta;
    double sin_theta;
    double cos_theta;
    double sin_phi;
    double cos_phi;
    double az;
    double el;
    double Lxh;
    double Lyh;
    double Lzh;
    double Sx;
    double Ex;
    double Zx;
    double Sy;
    double Ey;
    double Zy;
    double Sz;
    double Ez;
    double Zz;
    double Lx;
    double Ly;
    double Lz;
    double cos_delta;
    double sin_alpha;
    double cos_alpha;

    Calculate_Obs( time, pos, vel, geodetic, obs_set );

    az = obs_set->x;
    el = obs_set->y;
    phi = geodetic->lat;
    theta = FMod2p( ThetaG_JD( time ) + geodetic->lon );
    sin_theta = sin( theta );
    cos_theta = cos( theta );
    sin_phi = sin( phi );
    cos_phi = cos( phi );
    Lxh = -cos( az ) * cos( el );
    Lyh = sin( az ) * cos( el );
    Lzh = sin( el );
    Sx = sin_phi * cos_theta;
    Ex = -sin_theta;
    Zx = cos_theta * cos_phi;
    Sy = sin_phi * sin_theta;
    Ey = cos_theta;
    Zy = sin_theta * cos_phi;
    Sz = -cos_phi;
    Ez = 0.0;
    Zz = sin_phi;
    Lx = ( Sx * Lxh ) + ( Ex * Lyh ) + ( Zx * Lzh );
    Ly = ( Sy * Lxh ) + ( Ey * Lyh ) + ( Zy * Lzh );
    Lz = ( Sz * Lxh ) + ( Ez * Lyh ) + ( Zz * Lzh );
    obs_set->y = asin_( Lz ); /* Declination (radians) */
    cos_delta = sqrt( 1.0 - Sqr( Lz ) );
    sin_alpha = Ly / cos_delta;
    cos_alpha = Lx / cos_delta;
    obs_set->x = atan2( sin_alpha, cos_alpha ); /* Right Ascension (radians) */
    obs_set->x = FMod2p( obs_set->x );
}

/* .... SGP4/SDP4 functions end .... */

char * SubString( const char * string,
                  int32_t buffer_length,
                  char * output_buffer,
                  int32_t start,
                  int32_t end )
{
    int32_t x;
    int32_t y = 0;
    char * retp = NULL;

    if( ( end >= start ) &&
        ( buffer_length > ( end - start + ( int32_t ) 2 ) ) )
    {
        for( x = start; ( x <= end ) && ( string[ x ] != '\0' ); x++ )
        {
            if( string[ x ] != ' ' )
            {
                output_buffer[ y ] = string[ x ];
                y++;
            }
        }

        output_buffer[ y ] = 0;
        retp = output_buffer;
    }

    return retp;
}

double acos_( double arg )
{
    return acos( ( arg < -1.0 ) ? -1.0 : ( ( arg > 1.0 ) ? 1.0 : arg ) );
}

double asin_( double arg )
{
    return asin( ( arg < -1.0 ) ? -1.0 : ( ( arg > 1.0 ) ? 1.0 : arg ) );
}
