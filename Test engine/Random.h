#pragma once
#include <random>
#include <chrono>
	using namespace std;
namespace JR_Random
{

	//A class that contains methods and properties for generating true and psuedo random numbers.
	class Random
	{

	public:
		//defualt constructor-- creates a new instance of Rnadom with defualt parameters
		Random();

		//deconstructor-- deletes dynamic memory for Random
		~Random();

		//initializes random
		//min- the minimum random number
		//max- the maximum random number
		void init(int min,int max);

		//getRandom-- returns a psuedo random number
		// uses the current time chrono to get seed
		int getRandom();

		//getTrueRandom-- returns a true random number
		//uses the random device to get seed.
		int getTrueRandom();


		//getMax-- returns the max random value
		int getMax();

		//getMin-- returns the minumum random value
		int getMin();

		//setMin-- sets the minimum random value
		void setMin(int min);

		//setMax-- sets the maximum random value
		void setMax(int max);
	private:
		//min- the minimum value to generate
		 int m_min;
		//max- the maximum number to generate
		 int m_max;
	};
}