/*--------------------------------------------------------------*/
/* 																*/
/*					output_zone						*/
/*																*/
/*	output_zone - creates output files objects.		*/
/*																*/
/*	NAME														*/
/*	output_zone - outputs current contents of a zone.			*/
/*																*/
/*	SYNOPSIS													*/
/*	void	output_zone(										*/
/*					struct	zone_object	*zone,				*/
/*					struct	date	date,  						*/
/*					FILE 	*outfile)							*/
/*																*/
/*	OPTIONS														*/
/*																*/
/*	DESCRIPTION													*/
/*																*/
/*	outputs spatial structure according to commandline			*/
/*	specifications to specific files							*/
/*																*/
/*	PROGRAMMER NOTES											*/
/*																*/
/*	We only permit one fileset per spatial modelling level.     */
/*	Each fileset has one file for each timestep.  				*/
/*																*/
/*--------------------------------------------------------------*/
#include <stdio.h>
#include "rhessys.h"

void	output_zone(	int basinID, int hillID,
					struct	zone_object	*zone,
					struct	date	current_date,
                    FILE *outfile
#ifdef JMG_TRACKING
                    ,struct simtime *simtime
#endif
                        )
{
	/*------------------------------------------------------*/
	/*	Local Function Declarations.						*/
	/*------------------------------------------------------*/

	/*------------------------------------------------------*/
	/*	Local Variable Definition. 							*/
	/*------------------------------------------------------*/

    char out_basic[] = "%d %d %d %d %d %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\n";

#ifdef JMG_TRACKING
    char out_format[1000] = "%d %d %d ";
    strcat(out_format,out_basic);
#else
    char out_format[1000] = "";
    strcat(out_format, out_basic);
#endif

    fprintf(outfile, out_format,

#ifdef JMG_TRACKING
        simtime->sday,
        simtime->smth,
        simtime->syr,
#endif

        current_date.day,
		current_date.month,
        current_date.year,
		basinID,
		hillID,
		zone[0].ID,
		zone[0].rain * 1000.0,
		zone[0].snow * 1000.0,
		zone[0].metv.tmax,
		zone[0].metv.tmin,
		zone[0].metv.vpd,
		zone[0].Kdown_direct,
		zone[0].Kdown_diffuse,
		zone[0].PAR_direct,
		zone[0].PAR_diffuse,
		zone[0].Ldown,
		zone[0].relative_humidity,
		zone[0].aspect,
		zone[0].z,
		zone[0].slope,
		zone[0].e_horizon,
		zone[0].w_horizon,
			zone[0].tdewpoint,
			zone[0].e_dewpoint,
			zone[0].atm_trans,
			zone[0].wind,
			zone[0].Delta_T,
			zone[0].defaults[0][0].sea_level_clear_sky_trans + zone[0].z * zone[0].defaults[0][0].atm_trans_lapse_rate,
			0.036 * exp(-0.154 * zone[0].Delta_T),
			zone[0].cloud_fraction,
			zone[0].CO2); //NREN20181115
	return;
} /*end output_zone*/
