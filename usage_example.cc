/*
 * usage_example.cc
 *
 *  Created on: Mar 1, 2013
 *      Author: mukul
 */
#include "opencv_gui.h"

using namespace std;

void button_callback_1();
void button_callback_2();
void checkbox_callback_1(bool value);
void checkbox_callback_2(bool value);

int main()
{
    // create an OpenCVGUI instance
	OpenCVGUI gui((char*) "My New GUI");

	// add 2 buttons
	gui.AddButton("Button 1", button_callback_1);
    gui.AddButton("Button 2", button_callback_2);

    // add 2 check boxes
    gui.AddCheckBox("CheckBox 1", checkbox_callback_1);
    gui.AddCheckBox("CheckBox 2", checkbox_callback_2);

    // prevent the program from exiting
    int temp;
	cin >> temp;

	return 0;
}

void button_callback_1()
{
    cout<<"Button 1 got pressed"<<endl;
}

void button_callback_2()
{
    cout<<"Button 2 got pressed"<<endl;
}

void checkbox_callback_1(bool value)
{
    cout<<"CheckBox 1 value:"<<value<<endl;
}

void checkbox_callback_2(bool value)
{
    cout<<"CheckBox 2 value:"<<value<<endl;
}

