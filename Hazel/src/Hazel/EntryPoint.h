#pragma once

#ifdef HZ_PLATFORM_WINDOWS	

extern Hazel::Application* Hazel::CreateApplication();
	
int main(int argc, char** argv)
{
	Hazel::Log::Init();
	HZ_CORE_WARN("Initialized Core Logger! ");
	int a = 5;
	HZ_TRACE("Initialized Client Logger! ");

	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;

	return 0;
}


#else
	#error Hazel only supports windows!
#endif 