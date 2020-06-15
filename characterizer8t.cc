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

characterizer8t::characterizer8t(unsigned int inTech)
{
	technology = inTech;
}

unsigned int 
characterizer8t::get_technology()
{
	return technology;
}

unsigned int 
characterizer8t::get_delay()
{
	return delay;
}


//TODO: Input order (nm, pm) and divide it as it should
//Now always 
bool 
characterizer8t::set_technology(unsigned int input)
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
	printf("Tech %d\n",technology);
	accesNmos1->width = 8 * technology;
    inverterNmos1->width = 4 * technology;
    inverterPmos1->width = 3 * technology;


	accesNmos2->width = 8 * technology;
    inverterNmos2->width = 4 * technology;
    inverterPmos2->width = 3 * technology;

	accesNmos1->length = 2 * technology;
    inverterNmos1->length = 2 * technology;
    inverterPmos1->length = 3 * technology; 

	accesNmos2->length = 2 * technology;
    inverterNmos2->length = 2 * technology;
    inverterPmos2->length = 3 * technology;  

	printf("accesNmos1->width %d\n",accesNmos1->width);
	printf("accesNmos1->length %d\n",accesNmos1->length);
	printf("inverterNmos2->width %d\n",inverterNmos2->width);
	printf("inverterNmos2->length %d\n",inverterNmos2->length);
	printf("inverterPmos2->width %d\n",inverterPmos2->width);
	printf("inverterPmos2->length %d\n",inverterPmos2->length);
    //TODO: Verbose 
    //TODO: handling errors

    return true;
}

bool
characterizer8t::characterizeReadTransistors(Transistor* M1,Transistor* M2,Transistor* M3,Transistor* M4,
      Transistor* M5,Transistor* M6,Transistor* M7,Transistor* M8)
{
	unsigned int C5,C6,Cx,R6,R3,Rx;
	bool find = false;
	bool afin,bfin,cfin,dfin = false;
	unsigned int MAX = 24;

	unsigned int it = 0;
	unsigned int i,j;
	unsigned int a,b,c,d = 1;
	
	unsigned aSol[MAX];
	unsigned bSol[MAX];
	unsigned cSol[MAX];
	unsigned dSol[MAX];

	C5 = (M5->width * M5->length);
	C6 = (M6->width * M6->length);

	Cx = C5 * C6;
	//C7

	R6 = (M6->length / M6->width);
	R3 = (M3->length / M3->width);

	Rx = R6 + R3;
	printf("hi");
	printf("sol1: L7 * W7 == %d\n", Cx);
	printf("sol1: L7 / W7 + L8 / W8 == %d\n", Rx);

/*
	//SOLVE
	for(i= 1; i < MAX; i++) {
		for(j = 0; j < 5; j++){
			a = i + (j == 0);
			b = i + (j == 1);
			c = i + (j == 2);
			d = i + (j == 3);
			printf ("%d .. %d .. %d .. %d\n",a,b,c,d);

			if ((a*b == Cx) && ((a/b)+(c/d) == Rx)){
				aSol[it] = a;
				bSol[it] = b;
				cSol[it] = c;
				dSol[it] = d;

				it++;
			}
		}
	}

	printf("Find %d solutions!\n", it);*/
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
	Transistor* M7;
	Transistor* M8;

	bool ret;

	characterize6t(M1,M2,M3,M4,M5,M6);
	characterizeReadTransistors(M1,M2,M3,M4,M5,M6,M7,M8);

	//TODO hardcoded values should be a list, do the permutations, get the values and give the best characterization
 	return true;
}

