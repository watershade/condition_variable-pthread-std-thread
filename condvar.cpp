#include <iostream>
#include <stdio.h>
//#include <unistd.h>
#include <thread>
#include <mutex>
#include <condition_variable>
//#include <chrono>

using namespace std;

volatile bool isNewString = false;
string strShare = "";

condition_variable cvOut, cvIn;
mutex mtxNS;

bool showNSOneShot(string &content)
{
    //unique_lock<mutex> lck(mtxNS, defer_lock); // remeber defer_lock will not lock mutex and you need lock it manual;
    //lck.lock();
    unique_lock<mutex> lck(mtxNS);  // it will lock mutex at once
    while (!isNewString)
        cvOut.wait(lck, [&] { return isNewString; });
    cout << "We get new content:\r\n  " << content << endl;
    if (content.compare("Exit") == 0)
    {
        cout << " Thread tNS will stop ..." << endl;
        notify_all_at_thread_exit(cvIn, std::move(lck));
        return false;
    }
    isNewString = false;
    cvIn.notify_one();
    return true;
}

void showNS(string &content)
{

    //int circle = 0;
    bool inCircle = true;

    while (inCircle)
    {
        inCircle = showNSOneShot(ref(content));
    }
    cout << "tNS Exit" << endl;
}

int main(void)
{

    string voiceCmd[11];
    voiceCmd[0].assign("It is a start, we will fulfill our target!");
    voiceCmd[1].assign("2nd step is to think about the way to approach");
    voiceCmd[2].assign("3rd prepare your heart to fright with resistance");
    voiceCmd[3].assign("4th think about why you fail or succeed");
    voiceCmd[4].assign("5th no one can stop you");
    voiceCmd[5].assign("6th more ideas and more actions");
    voiceCmd[6].assign("7th what will you do in your next step");
    voiceCmd[7].assign("8th help the people they need help when you have the ability");
    voiceCmd[8].assign("9th Do not forget what is life");
    voiceCmd[9].assign("10th It is a happy endding");
    //voiceCmd[9].assign("Exit");
    voiceCmd[10].assign("Exit");

    thread tNS(showNS, ref(strShare));

    isNewString = false;

    for (int i = 0; i < 11; i++)
    {
        //this_thread::sleep_for(std::chrono::milliseconds(25));
        //unique_lock<mutex> lck(mtxNS, defer_lock);
        //lck.lock();
        unique_lock<mutex> lck(mtxNS);
        while (isNewString)
            cvIn.wait(lck, [&] { return (!isNewString); });

        strShare.assign(voiceCmd[i]);

        cout << "Main Thread #" << i << " is running..." << endl;
        isNewString = true;
        cvOut.notify_one();
        //this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    if (tNS.joinable())
    {
        //sleep(1);
        //cout << "strShare is: "<< strShare << endl;
        //cout << "ifNewString is: " << isNewString <<endl;
        //isNewString = true;
        //strShare.assign(voiceCmd[10]);
        //cvOut.notify_all();
        cout << "Will join tNS" << endl;
        tNS.join();
    }

    cout << "Hardware concurrency is " << thread::hardware_concurrency() << endl;
    cout << "Finish..." << endl;
    return 0;
}
