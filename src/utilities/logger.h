#ifndef __LOGGER_H__
#define __LOGGER_H__

class c_logger
{
public:

	c_logger();
	virtual ~c_logger();

	void error(const char *format, ...);

private:

};

#endif // __LOGGER_H__