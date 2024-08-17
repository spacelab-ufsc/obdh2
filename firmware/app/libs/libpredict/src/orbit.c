#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <predict/defs.h>
#include <predict/sdp4.h>
#include <predict/sgp4.h>
#include <predict/sun.h>
#include <predict/unsorted.h>

static bool is_eclipsed( const double pos[ 3 ],
                         const double sol[ 3 ],
                         double * depth );

static bool predict_decayed( const predict_orbital_elements_t * orbital_elements,
                             predict_julian_date_t time );

static int32_t parse_tle_field_i32( const char * tle_sub_string,
                                    int32_t * param );
static int32_t parse_tle_field_i64( const char * tle_sub_string,
                                    int64_t * param );
static int32_t parse_tle_field_f64( const char * tle_sub_string,
                                    double * param );

// length of buffer used for extracting subsets of TLE strings for parsing
#define SUBSTRING_BUFFER_LENGTH 50

predict_orbital_elements_t * predict_parse_tle(
    predict_orbital_elements_t * orbital_elements,
    struct predict_sgp4 * sgp4,
    struct predict_sdp4 * sdp4,
    const char * tle_line_1,
    const char * tle_line_2 )
{
    double tempnum = 0.0;
    int32_t err = 0;
    predict_orbital_elements_t * m = NULL;

    if( ( orbital_elements != NULL ) && ( tle_line_1 != NULL ) &&
        ( tle_line_2 != NULL ) )
    {
        m = orbital_elements;

        char substring_buffer[ SUBSTRING_BUFFER_LENGTH ];
        err = parse_tle_field_i32( SubString( tle_line_1,
                                              SUBSTRING_BUFFER_LENGTH,
                                              substring_buffer,
                                              2,
                                              6 ),
                                   &m->satellite_number );
        if( err == 0 )
        {
            err = parse_tle_field_i64( SubString( tle_line_1,
                                                  SUBSTRING_BUFFER_LENGTH,
                                                  substring_buffer,
                                                  64,
                                                  67 ),
                                       &m->element_number );
        }

        if( err == 0 )
        {
            err = parse_tle_field_i32( SubString( tle_line_1,
                                                  SUBSTRING_BUFFER_LENGTH,
                                                  substring_buffer,
                                                  18,
                                                  19 ),
                                       &m->epoch_year );
        }

        if( err == 0 )
        {
            ( void ) strncpy( m->designator,
                              SubString( tle_line_1,
                                         SUBSTRING_BUFFER_LENGTH,
                                         substring_buffer,
                                         9,
                                         16 ),
                              8 );
        }

        if( err == 0 )
        {
            err = parse_tle_field_f64( SubString( tle_line_1,
                                                  SUBSTRING_BUFFER_LENGTH,
                                                  substring_buffer,
                                                  20,
                                                  31 ),
                                       &m->epoch_day );
        }

        if( err == 0 )
        {
            err = parse_tle_field_f64( SubString( tle_line_2,
                                                  SUBSTRING_BUFFER_LENGTH,
                                                  substring_buffer,
                                                  8,
                                                  15 ),
                                       &m->inclination );
        }

        if( err == 0 )
        {
            err = parse_tle_field_f64( SubString( tle_line_2,
                                                  SUBSTRING_BUFFER_LENGTH,
                                                  substring_buffer,
                                                  17,
                                                  24 ),
                                       &m->right_ascension );
        }

        if( err == 0 )
        {
            err = parse_tle_field_f64( SubString( tle_line_2,
                                                  SUBSTRING_BUFFER_LENGTH,
                                                  substring_buffer,
                                                  26,
                                                  32 ),
                                       &m->eccentricity );
            m->eccentricity = m->eccentricity * 1.0e-07;
        }

        if( err == 0 )
        {
            err = parse_tle_field_f64( SubString( tle_line_2,
                                                  SUBSTRING_BUFFER_LENGTH,
                                                  substring_buffer,
                                                  34,
                                                  41 ),
                                       &m->argument_of_perigee );
        }

        if( err == 0 )
        {
            err = parse_tle_field_f64( SubString( tle_line_2,
                                                  SUBSTRING_BUFFER_LENGTH,
                                                  substring_buffer,
                                                  43,
                                                  50 ),
                                       &m->mean_anomaly );
        }

        if( err == 0 )
        {
            err = parse_tle_field_f64( SubString( tle_line_2,
                                                  SUBSTRING_BUFFER_LENGTH,
                                                  substring_buffer,
                                                  52,
                                                  62 ),
                                       &m->mean_motion );
        }

        if( err == 0 )
        {
            err = parse_tle_field_f64( SubString( tle_line_1,
                                                  SUBSTRING_BUFFER_LENGTH,
                                                  substring_buffer,
                                                  33,
                                                  42 ),
                                       &m->derivative_mean_motion );
        }

        if( err == 0 )
        {
            err = 1.0e-5 *
                  parse_tle_field_f64( SubString( tle_line_1,
                                                  SUBSTRING_BUFFER_LENGTH,
                                                  substring_buffer,
                                                  44,
                                                  49 ),
                                       &tempnum );
            tempnum = 1.0e-5 * tempnum;
        }

        if( err == 0 )
        {
            m->second_derivative_mean_motion = tempnum /
                                               pow( 10.0,
                                                    ( tle_line_1[ 51 ] -
                                                      '0' ) );

            err = parse_tle_field_f64( SubString( tle_line_1,
                                                  SUBSTRING_BUFFER_LENGTH,
                                                  substring_buffer,
                                                  53,
                                                  58 ),
                                       &tempnum );
            tempnum = 1.0e-5 * tempnum;
        }

        if( err == 0 )
        {
            m->bstar_drag_term = tempnum /
                                 pow( 10.0, ( tle_line_1[ 60 ] - '0' ) );

            err = parse_tle_field_i32( SubString( tle_line_2,
                                                  SUBSTRING_BUFFER_LENGTH,
                                                  substring_buffer,
                                                  63,
                                                  67 ),
                                       &m->revolutions_at_epoch );
        }

        if( err == 0 )
        {
            /* Period > 225 minutes is deep space */
            double ao;
            double xnodp;
            double dd1;
            double dd2;
            double delo;
            double a1;
            double del1;
            double r1;
            double temp = TWO_PI / MINUTES_PER_DAY / MINUTES_PER_DAY;
            double xno = m->mean_motion * temp *
                         MINUTES_PER_DAY; // from old TLE struct
            dd1 = ( XKE / xno );
            dd2 = TWO_THIRD;
            a1 = pow( dd1, dd2 );
            r1 = cos( m->inclination * M_PI / 180.0 );
            dd1 = ( 1.0 - ( m->eccentricity * m->eccentricity ) );
            temp = CK2 * 1.5f * ( ( r1 * r1 * 3.0 ) - 1.0 ) / pow( dd1, 1.5 );
            del1 = temp / ( a1 * a1 );
            ao = a1 *
                 ( 1.0 - ( del1 * ( ( TWO_THIRD * 0.5 ) +
                                    ( del1 * ( ( del1 * 1.654320987654321 ) +
                                               1.0 ) ) ) ) );
            delo = temp / ( ao * ao );
            xnodp = xno / ( delo + 1.0 );

            /* Select a deep-space/near-earth ephemeris */
            if( ( TWO_PI / xnodp / MINUTES_PER_DAY ) >= 0.15625 )
            {
                m->ephemeris = EPHEMERIS_SDP4;

                m->ephemeris_data = sdp4;

                if( m->ephemeris_data == NULL )
                {
                    m = NULL;
                }
                else
                {
                    // Initialize ephemeris data structure
                    sdp4_init( m, m->ephemeris_data );
                }
            }
            else
            {
                m->ephemeris = EPHEMERIS_SGP4;

                m->ephemeris_data = sgp4;

                if( m->ephemeris_data == NULL )
                {
                    m = NULL;
                }
                else
                {
                    // Initialize ephemeris data structure
                    sgp4_init( m, m->ephemeris_data );
                }
            }
        }
        else
        {
            m = NULL;
        }
    }

    return m;
}

bool predict_is_geosynchronous( const predict_orbital_elements_t * m )
{
    return ( m->mean_motion >= GEOSYNCHRONOUS_LOWER_MEAN_MOTION ) &&
           ( m->mean_motion <= GEOSYNCHRONOUS_UPPER_MEAN_MOTION ) &&
           ( fabs( m->eccentricity ) <=
             GEOSYNCHRONOUS_ECCENTRICITY_THRESHOLD ) &&
           ( fabs( m->inclination ) <=
             GEOSYNCHRONOUS_INCLINATION_THRESHOLD_DEGREES );
}

double predict_apogee( const predict_orbital_elements_t * m )
{
    double sma = 331.25 * exp( log( 1440.0 / m->mean_motion ) * ( 2.0 / 3.0 ) );
    return sma * ( 1.0 + m->eccentricity ) - EARTH_RADIUS_KM_WGS84;
}

double predict_perigee( const predict_orbital_elements_t * m )
{
    double xno = m->mean_motion * TWO_PI / MINUTES_PER_DAY;
    double a1 = pow( XKE / xno, TWO_THIRD );
    double cosio = cos( m->inclination * M_PI / 180.0 );
    double theta2 = cosio * cosio;
    double x3thm1 = ( 3.0 * theta2 ) - 1.0;
    double eosq = m->eccentricity * m->eccentricity;
    double betao2 = 1.0 - eosq;
    double betao = sqrt( betao2 );
    double del1 = 1.5 * CK2 * x3thm1 / ( a1 * a1 * betao * betao2 );
    double ao = a1 *
                ( 1.0 -
                  ( del1 * ( ( 0.5 * TWO_THIRD ) +
                             ( del1 * ( 1.0 + ( 134.0 / 81.0 * del1 ) ) ) ) ) );
    double delo = 1.5 * CK2 * x3thm1 / ( ao * ao * betao * betao2 );
    double aodp = ao / ( 1.0 - delo );

    return ( aodp * ( 1.0 - m->eccentricity ) - AE ) * EARTH_RADIUS_KM_WGS84;
}

bool predict_aos_happens( const predict_orbital_elements_t * m,
                          double latitude )
{
    /* This function returns true if the satellite pointed to by
       "x" can ever rise above the horizon of the ground station. */

    bool retval;
    double lin;
    double apogee;

    if( m->mean_motion == 0.0 )
    {
        retval = false;
    }
    else
    {
        lin = m->inclination;

        if( lin >= 90.0 )
        {
            lin = 180.0 - lin;
        }

        apogee = predict_apogee( m );

        if( ( acos( EARTH_RADIUS_KM_WGS84 /
                    ( apogee + EARTH_RADIUS_KM_WGS84 ) ) +
              ( lin * M_PI / 180.0 ) ) > fabs( latitude ) )
        {
            retval = true;
        }
        else
        {
            retval = false;
        }
    }

    return retval;
}

/* This is the stuff we need to do repetitively while tracking. */
/* This is the old Calc() function. */
int32_t predict_orbit( const predict_orbital_elements_t * orbital_elements,
                       struct predict_position * m,
                       double utc )
{
    int32_t err = 0;

    /* Satellite position and velocity vectors */
    vec3_set( m->position, 0, 0, 0 );
    vec3_set( m->velocity, 0, 0, 0 );

    m->time = utc;
    double julTime = utc;

    /* Convert satellite's epoch time to Julian  */
    /* and calculate time since epoch in minutes */
    double epoch = ( 1000.0 * orbital_elements->epoch_year ) +
                   orbital_elements->epoch_day;
    double jul_epoch = Julian_Date_of_Epoch( epoch );
    double tsince = ( julTime - jul_epoch ) * MINUTES_PER_DAY;

    /* Call NORAD routines according to deep-space flag. */
    struct model_output output;
    switch( orbital_elements->ephemeris )
    {
        case EPHEMERIS_SDP4:
            sdp4_predict( orbital_elements->ephemeris_data, tsince, &output );
            break;
        case EPHEMERIS_SGP4:
            sgp4_predict( orbital_elements->ephemeris_data, tsince, &output );
            break;
        default:
            err = -1;
            break;
    }

    if( err == 0 )
    {
        m->position[ 0 ] = output.pos[ 0 ];
        m->position[ 1 ] = output.pos[ 1 ];
        m->position[ 2 ] = output.pos[ 2 ];
        m->velocity[ 0 ] = output.vel[ 0 ];
        m->velocity[ 1 ] = output.vel[ 1 ];
        m->velocity[ 2 ] = output.vel[ 2 ];
        m->phase = output.phase;
        m->argument_of_perigee = output.omgadf;
        m->inclination = output.xinck;
        m->right_ascension = output.xnodek;

        /* TODO: Remove? Scale position and velocity vectors to km and km/sec */
        Convert_Sat_State( m->position, m->velocity );

        /* Calculate satellite Lat North, Lon East and Alt. */
        geodetic_t sat_geodetic;
        Calculate_LatLonAlt( utc, m->position, &sat_geodetic );

        m->latitude = sat_geodetic.lat;
        m->longitude = sat_geodetic.lon;
        m->altitude = sat_geodetic.alt;

        // Calculate solar position
        double solar_vector[ 3 ];
        sun_predict( m->time, solar_vector );

        // Find eclipse depth and if sat is eclipsed
        m->eclipsed = is_eclipsed( m->position,
                                   solar_vector,
                                   &m->eclipse_depth );

        // Calculate footprint
        m->footprint = 2.0 * EARTH_RADIUS_KM_WGS84 *
                       acos( EARTH_RADIUS_KM_WGS84 /
                             ( EARTH_RADIUS_KM_WGS84 + m->altitude ) );

        // Calculate current number of revolutions around Earth
        double temp = TWO_PI / MINUTES_PER_DAY / MINUTES_PER_DAY;
        double age = julTime - jul_epoch;
        double xno = orbital_elements->mean_motion * temp * MINUTES_PER_DAY;
        double xmo = orbital_elements->mean_anomaly * M_PI / 180.0;
        m->revolutions = ( int64_t ) floor(
                             ( ( ( xno * MINUTES_PER_DAY / ( M_PI * 2.0 ) ) +
                                 ( age * orbital_elements->bstar_drag_term ) ) *
                               age ) +
                             ( xmo / ( 2.0 * M_PI ) ) ) +
                         orbital_elements->revolutions_at_epoch;

        // calculate whether orbit is decayed
        m->decayed = predict_decayed( orbital_elements, utc );
    }

    return err;
}

static bool predict_decayed( const predict_orbital_elements_t * orbital_elements,
                             predict_julian_date_t time )
{
    double satepoch;
    satepoch = DayNum( 1, 0, orbital_elements->epoch_year ) +
               orbital_elements->epoch_day;

    bool has_decayed = false;
    if( satepoch +
            ( ( 16.666666 - orbital_elements->mean_motion ) /
              ( 10.0 * fabs( orbital_elements->derivative_mean_motion ) ) ) <
        time )
    {
        has_decayed = true;
    }
    return has_decayed;
}

/* Calculates if a position is eclipsed.  */
static bool is_eclipsed( const double pos[ 3 ],
                         const double sol[ 3 ],
                         double * depth )
{
    bool retval;
    double Rho[ 3 ];
    double earth[ 3 ];

    /* Determine partial eclipse */
    double sd_earth = asin_( EARTH_RADIUS_KM_WGS84 / vec3_length( pos ) );
    vec3_sub( sol, pos, Rho );
    double sd_sun = asin_( SOLAR_RADIUS_KM / vec3_length( Rho ) );
    vec3_mul_scalar( pos, -1, earth );

    double delta = acos_( vec3_dot( sol, earth ) / vec3_length( sol ) /
                          vec3_length( earth ) );
    *depth = sd_earth - sd_sun - delta;

    if( sd_earth < sd_sun )
    {
        retval = false;
    }
    else if( *depth >= 0 )
    {
        retval = true;
    }
    else
    {
        retval = false;
    }

    return retval;
}

double predict_squint_angle( const predict_observer_t * observer,
                             const struct predict_position * orbit,
                             double alon,
                             double alat )
{
    double bx = cos( alat ) * cos( alon + orbit->argument_of_perigee );
    double by = cos( alat ) * sin( alon + orbit->argument_of_perigee );
    double bz = sin( alat );

    double cx = bx;
    double cy = ( by * cos( orbit->inclination ) ) -
                ( bz * sin( orbit->inclination ) );
    double cz = ( by * sin( orbit->inclination ) ) +
                ( bz * cos( orbit->inclination ) );
    double ax = ( cx * cos( orbit->right_ascension ) ) -
                ( cy * sin( orbit->right_ascension ) );
    double ay = ( cx * sin( orbit->right_ascension ) ) +
                ( cy * cos( orbit->right_ascension ) );
    double az = cz;

    struct predict_observation obs;
    predict_observe_orbit( observer, orbit, &obs );
    double squint = acos( -( ( ax * obs.range_x ) + ( ay * obs.range_y ) +
                             ( az * obs.range_z ) ) /
                          obs.range );

    return squint;
}

static int32_t parse_tle_field_i32( const char * tle_sub_string,
                                    int32_t * param )
{
    int32_t err = 0;
    char * end_ptr = NULL;

    errno = 0;
    int64_t tmp = ( int64_t ) strtoll( tle_sub_string, &end_ptr, 10 );

    if (errno != 0)
    {
        err = -1;
    }

    if( end_ptr == tle_sub_string )
    {
        err = -1;
    }

    if( ( tmp > INT32_MAX ) || ( tmp < INT32_MIN ) )
    {
        err = -1;
    }

    if( err == 0 )
    {
        *param = ( int32_t ) tmp;
    }

    return err;
}

static int32_t parse_tle_field_i64( const char * tle_sub_string,
                                    int64_t * param )
{
    int32_t err = 0;
    char * end_ptr = NULL;

    errno = 0;
    int64_t tmp = ( int64_t ) strtoll( tle_sub_string, &end_ptr, 10 );

    if (errno != 0)
    {
        err = -1;
    }

    if( end_ptr == tle_sub_string )
    {
        err = -1;
    }

    if( err == 0 )
    {
        *param = ( int64_t ) tmp;
    }

    return err;
}

static int32_t parse_tle_field_f64( const char * tle_sub_string,
                                    double * param )
{
    int32_t err = 0;
    char * end_ptr = NULL;

    errno = 0;
    double tmp = ( double ) strtod( tle_sub_string, &end_ptr );

    if (errno != 0)
    {
        err = -1;
    }

    if( end_ptr == tle_sub_string )
    {
        err = -1;
    }

    if( err == 0 )
    {
        *param = tmp;
    }

    return err;
}
