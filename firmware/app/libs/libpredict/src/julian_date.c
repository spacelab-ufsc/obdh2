/*
 * This Julian Date implementation belong to philcrump's fork of libpredict,
 * which is accessible in https:/\/github.com/philcrump/libpredict.
 */
#include <predict/predict.h>

#include <predict/defs.h>

#define SECONDS_IN_HOUR      ( ( double ) 3600.0 )
#define SECONDS_IN_DAY       ( ( double ) 86400.0 )
#define UNIX_EPOCH_IN_JULIAN ( ( double ) 2440587.5 )

predict_julian_date_t julian_from_timestamp( uint64_t timestamp )
{
    return ( ( double ) timestamp / SECONDS_IN_DAY ) + UNIX_EPOCH_IN_JULIAN;
}

predict_julian_date_t julian_from_timestamp_ms( uint64_t timestamp_ms )
{
    return ( ( double ) timestamp_ms / ( 1000 * SECONDS_IN_DAY ) ) +
           UNIX_EPOCH_IN_JULIAN;
}

uint64_t timestamp_from_julian( predict_julian_date_t date )
{
    uint64_t timestamp = 0;
    double tmp;

    if( date > UNIX_EPOCH_IN_JULIAN )
    {
        tmp = ( ( date - UNIX_EPOCH_IN_JULIAN ) * SECONDS_IN_DAY );
        timestamp = ( uint64_t ) tmp;
    }

    return timestamp;
}

uint64_t timestamp_ms_from_julian( predict_julian_date_t date )
{
    uint64_t timestamp = 0;
    double tmp;

    if( date > UNIX_EPOCH_IN_JULIAN )
    {
        tmp = ( ( date - UNIX_EPOCH_IN_JULIAN ) * ( 1000.0 * SECONDS_IN_DAY ) );
        timestamp = ( uint64_t ) tmp;
    }

    return timestamp;
}
