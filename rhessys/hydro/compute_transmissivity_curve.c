/*--------------------------------------------------------------*/
/* 								*/
/*		compute_transmissivity_curve			*/
/*								*/
/*	NAME							*/
/*	compute_transmissivity_curve - estimates transmissivity	*/
/*		assuming an exponential decay of transmivivity	*/
/*		with depth - note returned value is 		*/
/*		relative transmissivity (i.e per unit		*/
/*		Ksat at the surface)				*/
/*								*/
/*								*/
/*	SYNOPSIS						*/
/*	compute_transmissivity_curve(				*/
/*				double	,			*/
/*				double	,			*/
/*				double	)			*/
/*								*/
/*	returns:						*/
/*	transmissivity - (unitless) multiplier for Ksat0 	*/
/*		to calculate transmissivity over range of	*/
/*		depths specified				*/
/*								*/
/*	OPTIONS							*/
/*	double	m - Ksat decay parameter			*/
/*	double	z - (m) depth to the water table		*/
/*	double  D - maximum depth				*/
/*								*/
/*	DESCRIPTION						*/
/*								*/
/*	computes transmissivity multiplier over range of 	*/
/*	depths between z and D; assumes an exponential decay	*/
/*	of Ksat with depth (decay given by m) until z_layer1	*/
/*	and then a constant conductivity below that layer	*/
/*	given by Ksat at the surface				*/
/*	Note that if m is 0, we assume that Ksat is constant    */
/*	with depth						*/
/*								*/
/*								*/
/*	PROGRAMMER NOTES					*/
/*								*/
/*--------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include "rhessys.h"
#include "phys_constants.h"
#include "functions.h"

double 	*compute_transmissivity_curve( 
					double  gamma,
					struct patch_object *patch,
					struct command_line_object *command_line
					)
{

	
	/*--------------------------------------------------------------*/
	/*	Local function definition.				*/
	/*--------------------------------------------------------------*/
    //double compute_field_capacity(
    //	int,
    //	int,
    //	double,
    //	double,
    //	double,
    //	double,
    //	double,
    //	double,
    //	double,
    //	double,
    //	double);


	double	compute_delta_water(
		int,
		double,
		double,
		double,
		double,
		double);


	double	compute_z_final(
		int,
		double,
		double,
		double,
		double,
		double);

	 void    *alloc( size_t, char *, char *);


	/*--------------------------------------------------------------*/
	/*	Local variable definition.				*/
	/*--------------------------------------------------------------*/

	int didx,initial;
	double	lower, depth, m;
	double	lower_z, depth_z;
	double start, fclayer, potential_sat;
	double transmissivity_layer;
	double *transmissivity;

	m = patch[0].soil_defaults[0][0].m;

	transmissivity = (double *) alloc((patch[0].num_soil_intervals+1) * sizeof(double),
					"trans","compute_transmissivity_cuve");


	/*--------------------------------------------------------------*/
	/*	for do not include surface overland flow or detention   */
	/*	storage here						*/
	/*--------------------------------------------------------------*/

    double k_bottom = 1.0;
    if (m > ZERO) {
        k_bottom = exp(-patch[0].soil_defaults[0][0].soil_water_cap/m);
    }

	if (patch[0].soil_defaults[0][0].soil_water_cap > patch[0].soil_defaults[0][0].interval_size) {
		initial = patch[0].num_soil_intervals;
		depth = patch[0].soil_defaults[0][0].soil_water_cap;
		transmissivity[initial]=0.0;
		initial = initial-1;

		for (didx=initial; didx >= 0; didx -= 1) {
			lower = depth;
			depth = depth-patch[0].soil_defaults[0][0].interval_size;

			
            lower_z = compute_z_final_from_surface(
                patch[0].soil_defaults[0],
                -1.0*lower);

            depth_z = compute_z_final_from_surface(
                patch[0].soil_defaults[0],
                -1.0*depth);

            fclayer = compute_field_capacity(
				command_line[0].verbose_flag,
				patch[0].soil_defaults[0][0].theta_psi_curve,
				patch[0].soil_defaults[0][0].psi_air_entry,
				patch[0].soil_defaults[0][0].pore_size_index,
				patch[0].soil_defaults[0][0].p3,
				patch[0].soil_defaults[0][0].p4,
				patch[0].soil_defaults[0][0].porosity_0,
				patch[0].soil_defaults[0][0].porosity_decay,
                patch[0].soil_defaults[0][0].Dingman_coef,
				patch[0].soil_defaults[0][0].soil_depth,
				lower_z,
				depth_z);

            //printf("didx:%d fclayer:%f\n",didx,fclayer);


        if (m > ZERO) {
#ifndef LIU_GAMMA_TRANSMISSIVITY_NEW
			transmissivity_layer = gamma  
             * (exp ( -1.0 * (max(depth, 0.0)/ m)) - exp ( -1.0 * (lower/m)));  //(m3)
#else
            transmissivity_layer = m * (exp ( -1.0 * (max(depth, 0.0)/ m)) - exp ( -1.0 * (lower/m)))
                                   * patch[0].soil_defaults[0][0].Ksat_0; //(m2)
#endif
        } else {
#if !defined(LIU_GAMMA_TRANSMISSIVITY_NEW)
            transmissivity_layer =  gamma * (lower-depth); //(m3)
#else
            transmissivity_layer = patch[0].soil_defaults[0][0].Ksat_0 * (lower-depth); //(m2)
#endif

        }

		fclayer = max(patch[0].soil_defaults[0][0].interval_size-fclayer,0.0);
#ifndef LIU_GAMMA_TRANSMISSIVITY_NEW
        transmissivity_layer = min(fclayer, transmissivity_layer/patch[0].area);  //(m)
#else
        double pot_flow_m3 = transmissivity_layer * patch[0].innundation_list->gamma;
        if ((pot_flow_m3 / patch[0].area) > fclayer)
            transmissivity_layer = fclayer * patch[0].area / patch[0].innundation_list->gamma;
#endif

#ifndef LIU_GAMMA_TRANSMISSIVITY_NEW
        if (gamma > ZERO) {
			transmissivity[didx] = transmissivity[didx+1]+transmissivity_layer * 
				patch[0].area / gamma;
        } else {
			transmissivity[didx] = transmissivity[didx+1];
        }
#else
        transmissivity[didx] = transmissivity[didx+1] + transmissivity_layer;
#endif


        //printf("didx:%d transmissivity:%lf\n",didx,transmissivity[didx]);
        }

	}
	
	else {
		initial = 1;
		transmissivity[initial]=0.0;
		lower = patch[0].soil_defaults[0][0].soil_water_cap;
		depth = 0;
        //09092022LML: seems not right and need double check!
#ifndef LIU_GAMMA_TRANSMISSIVITY_NEW
		if (m > ZERO)
			transmissivity[initial-1] =   (exp ( -1.0 * (max(depth, 0.0)/ m)) - exp ( -1.0 * (lower/m))); 
		else
			transmissivity[initial-1] =  (lower-depth);
#else
        if (m > ZERO) transmissivity[initial-1] = m * ((1.0 - k_bottom) / lower) * patch[0].soil_defaults[0][0].Ksat_0;
        else transmissivity[initial-1] = (lower-depth) * patch[0].soil_defaults[0][0].Ksat_0;
#endif
		
		}

    return(transmissivity); //09122022LML (m2 H2O/day array)

} /*compute_transmissivity_curve*/
