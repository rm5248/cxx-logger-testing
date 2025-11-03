#include "logger.h"

Logger::Logger(const char* name, std::function<void(LogEvent& evt)> outputter)
{
    m_level = Logger::OFF;
    m_name = name;
    m_outputter = outputter;
}

void Logger::set_level(int level){
    m_level = level;
}
