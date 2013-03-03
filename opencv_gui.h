/*
 * opencv_gui.h
 *
 *  Created on: Mar 1, 2013
 *      Author: mukul
 */

#ifndef OPENCV_GUI_H_
#define OPENCV_GUI_H_
#include <iostream>
#include <opencv2/opencv.hpp>

// this OpenCVGUI class provides buttons/check box support in OpenCV highgui
// new buttons/check boxes can be created and a call
class OpenCVGUI
{

public:
    // window title for the button/check box group
    OpenCVGUI(std::string title = NULL);
    ~OpenCVGUI();

    // add new button and attach a callback function.
    // the callback must be of following format:
    // void button_callback();
    void AddButton(std::string text, void (*function_callback)());

    // add new button and attach a callback function.
    // the callback must be of following format:
    // void checkbox_callback(bool value);
    void AddCheckBox(std::string text, void (*function_callback)(bool));

private:

    std::string title_;

    // class which defines a checkbox/button element
    class GUIElements
    {
    public:
        GUIElements() :
                type_(0), button_callback(NULL), checkbox_callback(NULL), value_(false)
        {
            ;
        }
        ~GUIElements()
        {
            ;
        }
        std::string text_;
        int type_; // 0: Button, 1: Checkbox

        // for button
        void (*button_callback)();
        // for checkbox
        void (*checkbox_callback)(bool);
        bool value_; // true if pressed, else false
    };

    std::vector<GUIElements> elements_;

    struct MouseParams
    {
        int x;
        int y;
        int event;
    };

    // display window
    cv::Mat frame_;
    int frame_width_, frame_height_;
    static void mouse_callback(int event, int x, int y, int flags, void* param);
    MouseParams params_;

    // index: position from top, starting from 0
    int last_pressed_index_;

    // Display all elements with correct state(darker if pressed)
    void DisplayElements();

    // finds out which element (if any) was clicked
    void InterpretMouseEvents();

    // the run function is started in a new thread
    // it continuously draws gui elements
    void* Run(void);
    typedef void* (OpenCVGUI::*Thread2Ptr)(void);
    typedef void* (*PthreadPtr)(void*);

};

#endif /* OPENCV_GUI_H_ */
