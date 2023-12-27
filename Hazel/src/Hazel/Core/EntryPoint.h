#pragma once

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();
	
int main(int argc, char** argv)
{
	Hazel::Log::Init();
	HZ_CORE_INFO("Initialized Core Logger! "); 
	HZ_TRACE("Initialized Client Logger! ");

	auto app = Hazel::CreateApplication();
	app->Run();

	delete app;

	return 0;
}


#else
	#error Hazel only supports windows!
#endif 