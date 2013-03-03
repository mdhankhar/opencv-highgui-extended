/*
 * opencv_gui.cc
 *
 *  Created on: Mar 1, 2013
 *      Author: mukul
 */

#include "opencv_gui.h"
#include <pthread.h>
#include <stdio.h>

using namespace cv;

//===============================
void OpenCVGUI::mouse_callback(int event, int x, int y, int flags, void* param)
//===============================
{
    if (event == CV_EVENT_LBUTTONUP || event == CV_EVENT_LBUTTONDOWN)
    {
        MouseParams* p = (MouseParams*) param;
        p->x = x;
        p->y = y;
        p->event = event;
    }
}

//===============================
OpenCVGUI::OpenCVGUI(string title)
//===============================
{
    title_ = title;
    namedWindow(title_.c_str(), CV_WINDOW_AUTOSIZE);
    params_.event = -1;
    setMouseCallback(title_.c_str(), mouse_callback, &params_);
    frame_width_ = 400;
    frame_height_ = 20 + 50;
    frame_ = Mat::zeros(frame_height_, frame_width_, CV_8UC3);
    rectangle(frame_, Point(0, 0), Point(frame_width_, frame_height_), Scalar(249, 246, 241), -1, 8);

    // launch run thread
    Thread2Ptr t = &OpenCVGUI::Run;
    PthreadPtr p = *(PthreadPtr*) &t;
    pthread_t tid;
    if (pthread_create(&tid, 0, p, this) == 0) pthread_detach(tid);
}

//===============================
OpenCVGUI::~OpenCVGUI()
//===============================
{
    ;
}

//===============================
void OpenCVGUI::InterpretMouseEvents()
//===============================
{
    if (params_.event == CV_EVENT_LBUTTONUP)
    {
        params_.event = -1;
        // call button press callback
        if (elements_[last_pressed_index_].type_ == 0)
        {
            //button
            elements_[last_pressed_index_].value_ = false;
            elements_[last_pressed_index_].button_callback();
        }
        else
        {
            // checkbox
            elements_[last_pressed_index_].value_ = !elements_[last_pressed_index_].value_;
            elements_[last_pressed_index_].checkbox_callback(elements_[last_pressed_index_].value_);
        }
    }

    if (params_.event == CV_EVENT_LBUTTONDOWN)
    {
        // find out which button is pressed
        int y = params_.y - 10;
        last_pressed_index_ = y / 50;
        //cout<<"pressed_index:"<<pressed_index<<endl;
        if (elements_[last_pressed_index_].type_ == 0)
            elements_[last_pressed_index_].value_ = true; //button
    }
}

//===============================
void OpenCVGUI::DisplayElements()
//===============================
{
    // each element is 50 pixels high
    if ((int) elements_.size() > 0 && frame_height_ < 20 + 50 * ((int) elements_.size()))
    {
        frame_height_ = 20 + 50 * elements_.size();
        frame_ = Mat::zeros(frame_height_, frame_width_, CV_8UC3);
        rectangle(frame_, Point(0, 0), Point(frame_width_, frame_height_), Scalar(249, 246, 241), -1, 8);
    }

    // draw all elements
    for (int i = 0; i < (int) elements_.size(); i++)
    {
        if (elements_[i].value_)
            rectangle(frame_, Point(10, 10 + i * 50), Point(390, 50 + i * 50), Scalar(72, 146, 72), -1, CV_AA);
        else
            rectangle(frame_, Point(10, 10 + i * 50), Point(390, 50 + i * 50), Scalar(81, 163, 81), -1, CV_AA);
        putText(frame_, elements_[i].text_, Point(10, 35 + i * 50), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255, 255, 255), 1, CV_AA);

        // if check box, put a extra thin boundary rectangle
        if (elements_[i].type_ == 1)
            rectangle(frame_, Point(10, 10 + i * 50), Point(390, 50 + i * 50), Scalar(61, 123, 61), 2, CV_AA);
    }
}

//===============================
void* OpenCVGUI::Run(void)
//===============================
{
    while (true)
    {
        DisplayElements();
        InterpretMouseEvents();
        imshow(title_.c_str(), frame_);
        waitKey(50);
    }

    return NULL;
}

//===============================
void OpenCVGUI::AddButton(string text, void (*function_callback)())
//===============================
{
    GUIElements e;
    e.button_callback = function_callback;
    e.text_ = text;
    // center text
    if (e.text_.length() < 35)
    {
        int nrSpacesToAdd = 17 - e.text_.length() / 2;
        for (int i = 0; i < nrSpacesToAdd; i++)
            e.text_ = " " + e.text_;
    }
    elements_.push_back(e);
}

//===============================
void OpenCVGUI::AddCheckBox(string text, void (*function_callback)(bool))
//===============================
{
    GUIElements e;
    e.checkbox_callback = function_callback;
    e.type_ = 1;
    e.text_ = text;
    // center text
    if (e.text_.length() < 35)
    {
        int nrSpacesToAdd = 17 - e.text_.length() / 2;
        for (int i = 0; i < nrSpacesToAdd; i++)
            e.text_ = " " + e.text_;
    }
    elements_.push_back(e);
}
