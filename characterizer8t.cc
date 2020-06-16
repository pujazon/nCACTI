/*------------------------------------------------------------
 *                              CACTI 6.5
 *         Copyright 2008 Hewlett-Packard Development Corporation
 *                         All Rights Reserved
 *
 * Permission to use, copy, and modify this software and its documentation is
 * hereby granted only under the following terms and conditions.  Both the
 * above copyright notice and this permission notice must appear in all copies
 * of the software, derivative works or modified versions, and any portions
 * thereof, and both notices must appear in supporting documentation.
 *
 * Users of this software agree to the terms and conditions set forth herein, and
 * hereby grant back to Hewlett-Packard Company and its affiliated companies ("HP")
 * a non-exclusive, unrestricted, royalty-free right and license under any changes, 
 * enhancements or extensions  made to the core functions of the software, including 
 * but not limited to those affording compatibility with other hardware or software
 * environments, but excluding applications which incorporate this software.
 * Users further agree to use their best efforts to return to HP any such changes,
 * enhancements or extensions that they make and inform HP of noteworthy uses of
 * this software.  Correspondence should be provided to HP at:
 *
 *                       Director of Intellectual Property Licensing
 *                       Office of Strategy and Technology
 *                       Hewlett-Packard Company
 *                       1501 Page Mill Road
 *                       Palo Alto, California  94304
 *
 * This software may be distributed (but not offered for sale or transferred
 * for compensation) to third parties, provided such third parties agree to
 * abide by the terms and conditions of this notice.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND HP DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS.   IN NO EVENT SHALL HP 
 * CORPORATION BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 *------------------------------------------------------------*/

#include <fstream>
#include <iostream>
#include <sstream>


#include "io.h"
#include "area.h"
#include "basic_circuit.h"
#include "parameter.h"
#include "Ucache.h"
#include "nuca.h"
#include "characterizer8t.h"

using namespace std;

characterizer8t eightCell(0);

bool AreSame(double a, double b)
{
	//printf("a: %f; b: %f\n",a,b);
    return (a == b);
}

characterizer8t::characterizer8t(double inTech)
{
	technology = inTech;
    isCalculated = false;
}

double 
characterizer8t::get_technology()
{
	return technology;
}

double 
characterizer8t::get_delay()
{
	return delay;
}


//TODO: Input order (nm, pm) and divide it as it should
//Now always 
bool 
characterizer8t::set_technology(double input)
{
	technology = input;
	return (technology == input);
}

bool
characterizer8t::characterize6t(
	Transistor* inverterNmos1, Transistor* inverterPmos1,
	 Transistor* inverterNmos2, Transistor* inverterPmos2,
	 Transistor* accesNmos1, Transistor* accesNmos2)
{
	printf("Tech %f\n",technology);
	accesNmos1->width = 8;
    inverterNmos1->width = 4;
    inverterPmos1->width = 3;


	accesNmos2->width = 8;
    inverterNmos2->width = 4;
    inverterPmos2->width = 3;

	accesNmos1->length = 2;
    inverterNmos1->length = 2;
    inverterPmos1->length = 3; 

	accesNmos2->length = 2;
    inverterNmos2->length = 2;
    inverterPmos2->length = 3;  

	printf("accesNmos1->width %f\n",accesNmos1->width);
	printf("accesNmos1->length %f\n",accesNmos1->length);
	printf("inverterNmos2->width %f\n",inverterNmos2->width);
	printf("inverterNmos2->length %f\n",inverterNmos2->length);
	printf("inverterPmos2->width %f\n",inverterPmos2->width);
	printf("inverterPmos2->length %f\n",inverterPmos2->length);
    //TODO: Verbose 
    //TODO: handling errors

    return true;
}

bool
characterizer8t::findAllM7M8(double *Cx, double *Rx)
{
	int sol = 0;
	int a,b,c,d,i;
	bool cond1,cond2;
    Transistor vM7[N_MAX_SIZE]; 
    Transistor vM8[N_MAX_SIZE]; 

    for(i = 0; i < MAX_SIZE; i++) {
		vM7[sol].length = 0;
		vM7[sol].width 	= 0;
		vM8[sol].length = 0;
		vM8[sol].width 	= 0;
    }

	printf("Solve:\n");
	printf("(1) L7 * W7 == %f\n", *Cx);
	printf("(2) L7 / W7 + L8 / W8 == %f\n", *Rx);

	//Also hurts my eyes but...
	for(a=1; a < MAX_SIZE+1; a++) {
		for(b=1; b < MAX_SIZE+1; b++) {
			for(c=1; c < MAX_SIZE+1; c++) {	
				for(d=1;d < MAX_SIZE+1; d++) {
					vM7[sol].length = (double) a;
					vM7[sol].width 	= (double) b;
					vM8[sol].length = (double) c;
					vM8[sol].width 	= (double) d;

					cond1 = ((vM7[sol].length * vM7[sol].width) == (*Cx));
					cond2 = (((vM7[sol].length/vM7[sol].width)+(vM8[sol].length/vM8[sol].width)) == (*Rx));

					if((cond1 && cond2) && (sol < MAX_SIZE)) {
						sol++;
					}
				}
			}
		}
	}
	
	printf("Total solutions: %d\n",sol);
	for (i=0;i < sol; i++) {
		printf("L7: %f; W7: %f; L8: %f; W8: %f;\n",vM7[i].length,vM7[i].width,vM8[i].length,vM8[i].width);
	}
}

bool
characterizer8t::characterizeReadTransistors(Transistor* M3, Transistor* M5,Transistor* M6)
{
	double C5,C6,Cx,R6,R3,Rx;

	C5 = (M5->width * M5->length);
	C6 = (M6->width * M6->length);

	Cx = C5 + C6;
	//C7

	R6 = (M6->length / M6->width);
	R3 = (M3->length / M3->width);

	Rx = (double) R6 + R3;

	findAllM7M8(&Cx,&Rx);

    return true;
}

bool
characterizer8t::characterize()
{
	Transistor* M1;
	Transistor* M2;
	Transistor* M3;
	Transistor* M4;
	Transistor* M5;
	Transistor* M6;

	bool ret;

	if(!isCalculated) {
		characterize6t(M1,M2,M3,M4,M5,M6);
		characterizeReadTransistors(M3,M5,M6);
		isCalculated = true;
	} else {
		printf("Already calculated!\n");
	}

	//TODO hardcoded values should be a list, do the permutations, get the values and give the best characterization
 	return true;
}

