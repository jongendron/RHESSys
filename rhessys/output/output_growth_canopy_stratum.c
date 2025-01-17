/*--------------------------------------------------------------*/
/* 																*/
/*					output_growth_canopy_stratum						*/
/*																*/
/*	output_growth_canopy_stratum - creates output_growth files objects.		*/
/*																*/
/*	NAME														*/
/*	output_growth_canopy_stratum - output_growths  */
/*			current contents of a canopy_stratum.			*/
/*																*/
/*	SYNOPSIS													*/
/*	void	output_growth_canopy_stratum(										*/
/*					struct	canopy_stratum_object	*canopy_stratum,				*/
/*					struct	date	date,  						*/
/*					FILE 	*outfile)							*/
/*																*/
/*	OPTIONS														*/
/*																*/
/*	DESCRIPTION													*/
/*																*/
/*	output_growths spatial structure according to commandline			*/
/*	specificatiocs to specific files							*/
/*																*/
/*	PROGRAMMER NOTES											*/
/*																*/
/*	We only permit one fileset per spatial modelling level.     */
/*	Each fileset has one file for each timestep.  				*/
/*																*/
/*--------------------------------------------------------------*/
#include <stdio.h>
#include "rhessys.h"

void	output_growth_canopy_stratum( int basinID, int hillID, int zoneID,
									 int patchID,
									 struct	canopy_strata_object	*stratum,
									 struct	date	current_date,
                                     FILE *outfile
#ifdef JMG_TRACKING
                                    ,struct simtime *simtime
#endif
                                      )
{
	/*------------------------------------------------------*/
	/*	Local Function Declaratiocs.						*/
	/*------------------------------------------------------*/

	/*------------------------------------------------------*/
	/*	Local Variable Definition. 							*/
	/*------------------------------------------------------*/

    char out_basic[] = "%d %d %d %d %d %d %d %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\n";

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
		zoneID,
		patchID,
		stratum[0].ID,
		stratum[0].epv.proj_lai,
		stratum[0].epv.proj_lai_when_red, //beetle
		stratum[0].epv.proj_pai,
		stratum[0].epv.proj_pai_when_red,
		(stratum[0].cs.snagc + stratum[0].cs.delay_snagc)*1000,
		(stratum[0].ns.snagn + stratum[0].ns.delay_snagn)*1000,
		(stratum[0].cs.redneedlec + stratum[0].cs.delay_redneedlec)*1000,
		(stratum[0].ns.redneedlen + stratum[0].ns.delay_redneedlen)*1000, //end NREN 20180805
		(stratum[0].cs.dead_rootc_beetle)*1000,
		(stratum[0].ns.dead_rootn_beetle)*1000, //NREN 20190910
		(stratum[0].cs.leafc + stratum[0].cs.leafc_store + stratum[0].cs.leafc_transfer)* 1000.0,
        stratum[0].cs.leafc * 1000.,
        stratum[0].cs.leafc_store * 1000.,
        stratum[0].cs.leafc_transfer * 1000.,
		(stratum[0].ns.leafn + stratum[0].ns.leafn_store + stratum[0].ns.leafn_transfer)* 1000.0,
        stratum[0].ns.leafn * 1000.,
        stratum[0].ns.leafn_store * 1000.,
        stratum[0].ns.leafn_transfer * 1000.,
		stratum[0].cs.cpool * 1000.0,
		stratum[0].ns.npool * 1000.0,
		stratum[0].cs.dead_leafc * 1000.0,
		(stratum[0].cs.frootc + stratum[0].cs.frootc_store + stratum[0].cs.frootc_transfer)* 1000.0,
		(stratum[0].ns.frootn + stratum[0].ns.frootn_store + stratum[0].ns.frootn_transfer)* 1000.0,
		(stratum[0].cs.live_stemc + stratum[0].cs.livestemc_store + stratum[0].cs.livestemc_transfer)* 1000.0,
		(stratum[0].ns.live_stemn + stratum[0].ns.livestemn_store + stratum[0].ns.livestemn_transfer)* 1000.0,
		stratum[0].cs.leafc_store * 1000.0,
		stratum[0].ns.leafn_store * 1000.0,
		(stratum[0].cs.dead_stemc + stratum[0].cs.deadstemc_store + stratum[0].cs.deadstemc_transfer)* 1000.0,
		(stratum[0].ns.dead_stemn + stratum[0].ns.deadstemn_store + stratum[0].ns.deadstemn_transfer)* 1000.0,
		(stratum[0].cs.live_crootc + stratum[0].cs.livecrootc_store + stratum[0].cs.livecrootc_transfer)* 1000.0,
		(stratum[0].ns.live_crootn + stratum[0].ns.livecrootn_store + stratum[0].ns.livecrootn_transfer)* 1000.0,
		(stratum[0].cs.dead_crootc + stratum[0].cs.deadcrootc_store + stratum[0].cs.deadcrootc_transfer)* 1000.0,
		(stratum[0].ns.dead_crootn + stratum[0].ns.deadcrootn_store + stratum[0].ns.deadcrootn_transfer)* 1000.0,
		stratum[0].cs.cwdc * 1000.0,
		stratum[0].cdf.total_mr*1000.0,
		stratum[0].cdf.total_gr*1000.0,
		stratum[0].cdf.psn_to_cpool * 1000.0,
		stratum[0].cs.age,
		stratum[0].rootzone.depth*1000.0,
		stratum[0].phen.gwseasonday,
		stratum[0].phen.lfseasonday,
		stratum[0].phen.gsi,
		stratum[0].ns.nlimit,
        stratum[0].cdf.fleaf,
        stratum[0].cdf.froot,
        stratum[0].cdf.fwood,
        stratum[0].ndf.actual_N_uptake*1000.0,
        stratum[0].ndf.sminn_to_npool*1000.0,
        stratum[0].ndf.retransn_to_npool*1000.0,
        stratum[0].cs.mortality_fract,
        stratum[0].assim_sunlit,
        stratum[0].assim_shade,
        stratum[0].trans_sunlit*1000*86400,
        stratum[0].trans_shade*1000*86400,
        stratum[0].epv.proj_lai_sunlit,
        stratum[0].epv.proj_lai_shade);
	return;
} /*end output_growth_canopy_stratum*/
