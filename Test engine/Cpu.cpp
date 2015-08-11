#include "Cpu.h"

namespace JR_CPU
{
	//defualt constructor-- constructs a new instance of Cpu  with defualt parameters
	Cpu::Cpu()
	{
		//init variables
		m_canReadCpu = false;
		m_counterHandle = NULL;
		m_cpuUsage = 0;
		m_lastSampleTime = NULL;
		m_queryHandle = NULL;
	}
	//deconstructor-- deletes dynamic memory and cleans up the Cpu class
	Cpu::~Cpu()
	{
		//call shutdown
		shutdown();
	}

	//init-- initializes the Cpu class
	void Cpu::init()
	{
		PDH_STATUS status;

		//init flag indicating the cpu can be read
		m_canReadCpu = true;

		//create a query object to poll for cpu usage
		status = PdhOpenQuery(NULL, 0, &m_queryHandle);
		if (status != ERROR_SUCCESS)
		{
			m_canReadCpu = false;
		}

		//Set query object to poll all CPUs in the system
		status = PdhAddCounter(m_queryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &m_counterHandle);
		if (status != ERROR_SUCCESS)
		{
			m_canReadCpu = false;
		}

		m_lastSampleTime = GetTickCount();
		m_cpuUsage = 0;
	}

	//shudown-- shutdown the cpu class
	void Cpu::shutdown()
	{
		if (m_canReadCpu)
		{
			PdhCloseQuery(m_queryHandle);
		}
	}

	//frame-- processes a frame for the cpu class and updates it
	void Cpu::frame()
	{
		PDH_FMT_COUNTERVALUE value;

		if (m_canReadCpu)
		{
			if (m_lastSampleTime + 1000 < GetTickCount())
			{
				m_lastSampleTime = GetTickCount();

				PdhCollectQueryData(m_queryHandle);

				PdhGetFormattedCounterValue(m_counterHandle, PDH_FMT_LONG, NULL, &value);

				m_cpuUsage = value.longValue;
			}
		}
	}

	//getCpuPercentage-- returns the percentage the cpu is running at
	int Cpu::getCpuPercentage()
	{
		int usage;
		if (m_canReadCpu)
		{
			usage = (int)m_cpuUsage;
		}
		else
		{
			usage = 0;
		}

		return usage;
	}
}