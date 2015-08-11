#pragma once

//linkers
#pragma comment(lib,"pdh.lib")

//includes
#include <Pdh.h>
namespace JR_CPU
{
	class Cpu
	{
	public:
		//defualt constructor-- constructs a new instance of Cpu  with defualt parameters
		Cpu();
		//deconstructor-- deletes dynamic memory and cleans up the Cpu class
		~Cpu();

		//init-- initializes the Cpu class
		void init();

		//shudown-- shutdown the cpu class
		void shutdown();

		//frame-- processes a frame for the cpu class and updates it
		void frame();

		//getCpuPercentage-- returns the percentage the cpu is running at
		int getCpuPercentage();

	private:
		//canReadCpu- indicates whether the CPU can be read or not
		bool m_canReadCpu;
		//queryHandle- the hande for the cpu query
		HQUERY m_queryHandle;
		//couterHandle- the handle for the cpu counter
		HCOUNTER m_counterHandle;
		//lastSmapleTime- the time the CPU was last sampled from
		unsigned long m_lastSampleTime;
		//cpuUsage- the percentage the cpu is running at
		long m_cpuUsage;
	};
}