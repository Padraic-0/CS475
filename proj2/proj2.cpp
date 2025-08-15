#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

unsigned int seed = 0;
int	NowYear;		// 2023 - 2028
int	NowMonth;		// 0 - 11
int TotalMonths;    //0 - years * 12

float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// rye grass height in inches
int	NowNumRabbits;		// number of rabbits in the current population

const float RYEGRASS_GROWS_PER_MONTH =		20.0;
const float ONE_RABBITS_EATS_PER_MONTH =	 1.0;

const float AVG_PRECIP_PER_MONTH =	       12.0;	// average
const float AMP_PRECIP_PER_MONTH =		4.0;	// plus or minus
const float RANDOM_PRECIP =			2.0;	// plus or minus noise

const float AVG_TEMP =				60.0;	// average
const float AMP_TEMP =				20.0;	// plus or minus
const float RANDOM_TEMP =			10.0;	// plus or minus noise

const float MIDTEMP =				60.0;
const float MIDPRECIP =				14.0;

omp_lock_t	    Lock;
volatile int	NumInThreadTeam;
volatile int	NumAtBarrier;
volatile int	NumGone;



float Ranf( unsigned int *seedp,  float low, float high )
{
        float r = (float) rand_r( seedp );              // 0 - RAND_MAX

        return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}



float Sqr( float x )
{
        return x*x;
}

// specify how many threads will be in the barrier:
//	(also init's the Lock)


void InitBarrier( int n )
{
        NumInThreadTeam = n;
        NumAtBarrier = 0;
	    omp_init_lock( &Lock );
}


// have the calling thread wait here until all the other threads catch up:


void WaitBarrier( )
{
        omp_set_lock( &Lock );
        {
                NumAtBarrier++;
                if( NumAtBarrier == NumInThreadTeam )
                {
                        NumGone = 0;
                        NumAtBarrier = 0;
                        // let all other threads get back to what they were doing
			// before this one unlocks, knowing that they might immediately
			// call WaitBarrier( ) again:
                        while( NumGone != NumInThreadTeam-1 );
                        omp_unset_lock( &Lock );
                        return;
                }
        }
        omp_unset_lock( &Lock );

        while( NumAtBarrier != 0 );	// this waits for the nth thread to arrive

        #pragma omp atomic
        NumGone++;			// this flags how many threads have returned
}

//start
// starting date and time:

//end

//start

//end

//start

//end

//start

//end

//start


void Rabbits(){
    while( NowYear < 2029 )
    {
        int nextNumRabbits = NowNumRabbits;
        int carryingCapacity = (int)( NowHeight );
        if( nextNumRabbits < carryingCapacity )
                nextNumRabbits++;
        else
            if( nextNumRabbits > carryingCapacity )
                    nextNumRabbits--;

        if( nextNumRabbits < 0 )
                nextNumRabbits = 0;

        // DoneComputing barrier:
        WaitBarrier( );
        NowNumRabbits += nextNumRabbits;

        // DoneAssigning barrier:
        WaitBarrier( );
       

        // DonePrinting barrier:
        WaitBarrier( );

    }
}
void RyeGrass(){
    while( NowYear < 2029 )
    {
        float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

        float temp = AVG_TEMP - AMP_TEMP * cos( ang );
        

        float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
        
        

        float tempFactor = exp(   -Sqr(  ( NowTemp - MIDTEMP ) / 10.  )   );
        float precipFactor = exp(   -Sqr(  ( NowPrecip - MIDPRECIP ) / 10.  )   );
        float nextHeight = NowHeight;

        nextHeight += tempFactor * precipFactor * RYEGRASS_GROWS_PER_MONTH;
        nextHeight -= (float)NowNumRabbits * ONE_RABBITS_EATS_PER_MONTH;
        if( nextHeight < 0. ) 
            nextHeight = 0.;

        // DoneComputing barrier:
        WaitBarrier( );

        NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );
        NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
        if( NowPrecip < 0. )
            NowPrecip = 0.;
        NowHeight = nextHeight;

        // DoneAssigning barrier:
        WaitBarrier( );
        //printf("Height: %f\n", NowHeight);

        // DonePrinting barrier:
        WaitBarrier( );

    }
}
void Watcher(){
    while( NowYear < 2029 )
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:
        // DoneComputing barrier:
        // DoneComputing barrier:
        int temp_year = NowYear;
        int temp_month = NowMonth;
        TotalMonths ++;
        if (temp_month == 11){
            temp_month = 0;
            temp_year += 1;
        }else{
            temp_month += 1;
        }
        WaitBarrier( );
        
        NowYear = temp_year;
        NowMonth = temp_month;
        // DoneAssigning barrier:
        WaitBarrier( );
       
        //printf("Month: %d Year: %d\n", NowMonth, NowYear);

        printf("%d,%f,%d,%f,%f\n",TotalMonths, NowHeight * 2.54 / 10, NowNumRabbits,(5./9.)*(NowTemp -32), NowPrecip * 2.54 / 10);
        // DonePrinting barrier:
        WaitBarrier( );

    }
}
void MyAgent(){
    while( NowYear < 2029 )
    {
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:
         // DoneComputing barrier:
        // DoneComputing barrier:
        int nextNumRabbits = NowNumRabbits;
        float nextHeight = NowHeight;

        if (NowTemp > 70.0){
            float prairie_fire_intensity = (NowTemp / 30) - (NowPrecip / 30);
            int chance_of_fire = Ranf(&seed, 0, 10);
            if( chance_of_fire == 5 || chance_of_fire == 6 ){
                nextNumRabbits = round(nextNumRabbits / prairie_fire_intensity);
                nextHeight = nextHeight / prairie_fire_intensity;
            }

        }
        WaitBarrier( );

        NowNumRabbits -= nextNumRabbits;
        if (NowNumRabbits < 0){
            NowNumRabbits = 0;
        }
        NowHeight -= nextHeight;
        if (NowHeight < 0){
            NowHeight = 0;
        }

        NowPrecip += 20;
        

        // DoneAssigning barrier:
        WaitBarrier( );
       

        // DonePrinting barrier:
        WaitBarrier( );

    }
}


int main(void){
    NowMonth =    0;
    NowYear  = 2023;

    // starting state (feel free to change this if you want):
    NowNumRabbits = 1;
    NowHeight =  5.;

    omp_set_num_threads( 4 );	// or 4
    InitBarrier( 4 );		// or 4


    omp_set_num_threads( 4 );	// same as # of sections
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            Rabbits( );
        }

        #pragma omp section
        {
            RyeGrass( );
        }

        #pragma omp section
        {
            Watcher( );
        }

        #pragma omp section
        {
            MyAgent( );	// your own
        }
    }       // implied barrier -- all functions must return in order
        // to allow any of them to get past here
    return 0;
}

//clang++ -Xpreprocessor -fopenmp -I/usr/local/opt/libomp/include -L/usr/local/opt/libomp/lib -lomp proj2.cpp -o proj2