/*--------------------------------------------------------------*/
/* 																*/
/*					output_monthly_hillslope						*/
/*																*/
/*	output_monthly_hillslope - creates output files objects.		*/
/*																*/
/*	NAME														*/
/*	output_monthly_hillslope - outputs current contents of a hillslope.			*/
/*																*/
/*	SYNOPSIS													*/
/*	void	output_monthly_hillslope(										*/
/*					struct	hillslope_object	*hillslope,				*/
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

void	output_monthly_hillslope(	int basinID,
							 struct	hillslope_object	*hillslope,
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
	int check;
	if (hillslope[0].acc_month.length == 0) hillslope[0].acc_month.length = 1;

    char out_basic[] = "%d %d %d %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\n";

#ifdef JMG_TRACKING
    char out_format[1000] = "%d %d %d ";
    strcat(out_format,out_basic);
#else
    char out_format[1000] = "";
    strcat(out_format, out_basic);
#endif

    check = fprintf(outfile, out_format,

#ifdef JMG_TRACKING
            simtime->sday,
            simtime->smth,
            simtime->syr,
#endif

        current_date.month,
		current_date.year,
		basinID,
		hillslope[0].ID,
		hillslope[0].acc_month.streamflow * 1000.0,
		hillslope[0].acc_month.stream_NO3 * 1000.0,
		hillslope[0].acc_month.snowpack/hillslope[0].acc_month.length,
		hillslope[0].acc_month.denitrif * 1000.0,
		hillslope[0].acc_month.DOC_loss * 1000.0,
		hillslope[0].acc_month.DON_loss * 1000.0,
		hillslope[0].acc_month.et * 1000.0,
		hillslope[0].acc_month.psn * 1000.0,
		hillslope[0].acc_month.lai/hillslope[0].acc_month.length ,
		hillslope[0].acc_month.nitrif * 1000.0,
		hillslope[0].acc_month.mineralized * 1000.0,
		hillslope[0].acc_month.uptake * 1000.0,
		hillslope[0].area
		);
	if (check <= 0) {
		fprintf(stdout,
			"\nWARNING: output error has occured in output_monthly_hillslope");
	}
	/*--------------------------------------------------------------*/
	/*	reset accumulator variables				*/
	/*--------------------------------------------------------------*/
	hillslope[0].acc_month.snowpack = 0.0;
	hillslope[0].acc_month.streamflow = 0.0;
	hillslope[0].acc_month.stream_NO3 = 0.0;
	hillslope[0].acc_month.et = 0.0;
	hillslope[0].acc_month.psn = 0.0;
	hillslope[0].acc_month.length = 0;
	hillslope[0].acc_month.DOC_loss = 0.0;
	hillslope[0].acc_month.DON_loss = 0.0;
	hillslope[0].acc_month.denitrif= 0.0;
	hillslope[0].acc_month.lai = 0.0; 
	hillslope[0].acc_month.nitrif = 0.0; 
	hillslope[0].acc_month.mineralized = 0.0; 
	hillslope[0].acc_month.uptake = 0.0; 
	return;
} /*end output_monthly_hillslope*/
