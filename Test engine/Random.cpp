#include "Random.h"
namespace JR_Random
{
	//defualt constructor-- creates a new instance of Rnadom with defualt parameters
	Random::Random()
	{
		m_min = 0;
		m_max = 0xffffffff/2;
	}

	//deconstructor-- deletes dynamic memory for Random
	Random::~Random()
	{

	}

	//initializes random
	//min- the minimum random number
	//max- the maximum random number
	void Random::init(int min, int max)
	{
		m_min = min;
		m_max = max;
	}

	//getRandom-- returns a psuedo random number
	// uses the current time chrono to get seed
	int Random::getRandom()
	{
		//get a psudo random number using the mt19997 device
		typedef chrono::high_resolution_clock clock;
		//get the current time
		clock::time_point  beginning = clock::now();

		clock::duration duration = clock::now() - beginning;
		//sed the generator
		mt19937 generator;
		generator.seed(duration.count());

		uniform_int_distribution<int> dist(m_min,m_max);
		return dist(generator);
	}

	//getTrueRandom-- returns a true random number
	//uses the random device to get seed.
	int Random::getTrueRandom()
	{
		random_device rd;

		uniform_int_distribution<int> dist(m_min, m_max);

		return dist(rd);
	}


	//getMax-- returns the max random value
	int Random::getMax()
	{
		return m_max;
	}

	//getMin-- returns the minumum random value
	int Random::getMin()
	{
		return m_min;
	}

	//setMin-- sets the minimum random value
	//min- the new minimum value
	void Random::setMin(int min)
	{
		m_min = min;
	}

	//setMax-- sets the maximum random value
	//max- the new maximum value
	void Random::setMax(int max)
	{
		m_max = max;
	}
}