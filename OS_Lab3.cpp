#include <iostream>
#include "Menu.h"
#include "WinAPI_calculation.h"
#include "OpenMP_calculation.h"

auto form_menu() {
	using namespace std;
	auto main_menu = std::make_shared<Menu>("Main menu");
	main_menu->add_item("Calculation using the Win32 API", single_calc_winapi);
	main_menu->add_item("Multiple calculation using the Win32 API", multiple_calc_winapi);
	main_menu->add_item("Calculation using the OpenMP", single_calc_omp);
	main_menu->add_item("Multiple calculation using the OpenMP", multiple_calc_omp);

	main_menu->add_property(make_shared<string>("Number of threads"), make_shared<string>("16"));
	main_menu->add_property(make_shared<string>("Number of signs"), make_shared<string>("100000000"));
	main_menu->add_property(make_shared<string>("Block size"), make_shared<string>("930707"));
	main_menu->add_property(make_shared<string>("Number of calculations"), make_shared<string>("50"));

	return main_menu;
}

int main()
{
	form_menu()->execute();
}