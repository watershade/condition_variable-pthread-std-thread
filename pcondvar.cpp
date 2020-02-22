#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string>

using namespace std;

volatile bool isNewString = false;
string strShare = "";


pthread_cond_t cvOut, cvIn;
pthread_mutex_t mtxNS;

bool showNSOneShot()
{
    
    bool retval = true;
    //string content = *((string *)p2c);

    pthread_mutex_lock(&mtxNS);
    while (!isNewString)
        pthread_cond_wait(&cvOut, &mtxNS);

    cout << "We get new content:\r\n\t\t" << strShare << endl;
    if (strShare.compare("Exit") == 0)
    {
        cout << "Thread tNS will stop ..." << endl;
        retval = false;
    }
    cout << "will notify cvOut and unlock mtxNS\n~~~~~~~~~~~~~" << endl;
    isNewString = false;
    pthread_cond_signal(&cvIn);
    pthread_mutex_unlock(&mtxNS);
    return retval;
}

void *showNS(void*)
{

    //int circle = 0;
    bool inCircle = true;

    while (inCircle)
    {
        inCircle = showNSOneShot();
        //sleep(1);
    }
    cout << "tNS Exit" << endl;

    pthread_exit(NULL);
}

int main(void)
{
    pthread_t tNS;
    pthread_attr_t ptAttr;
    
    /*create thread tNS*/
    pthread_attr_init(&ptAttr);
    pthread_attr_setdetachstate(&ptAttr, PTHREAD_CREATE_JOINABLE);
    int rt = pthread_create(&tNS, &ptAttr, showNS, NULL);
    if (rt)
    {
        cerr << "Err/pthread: can not create pthread: tNS" << endl;
    }
    pthread_attr_destroy(&ptAttr);
    /*Dynamically initialize mtxNS */
    rt = pthread_mutex_init(&mtxNS, NULL);
    if (rt)
        cerr << "Err/pthread_mutex: can not initialize mtxNS" << endl;
    /*Dynamically initialize cvIn and cvOut*/
    rt = pthread_cond_init(&cvIn, NULL);
    if (rt)
        cerr << "Err/pthread_cond: can not initialize cvIn";
    rt = pthread_cond_init(&cvOut, NULL);
    if (rt)
        cerr << "Err/pthread_cond: can not initialize cvOut";

    /*fill some value for voiceCmd[] */
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

    isNewString = false;


    for (int i = 0; i < 11; i++)
    {
        pthread_mutex_lock(&mtxNS);
        //this_thread::sleep_for(std::chrono::milliseconds(25));
        //cvIn.wait(lck, [] { return (!isNewString); });
        while (isNewString)
            pthread_cond_wait(&cvIn, &mtxNS);

        /*fill message*/
        strShare.assign(voiceCmd[i]);
        cout << "Main Thread #" << i << " is running..." << endl;
    
        cout << "will notify cvOut and unlock mtxNS\n~~~~~~~~~~~~~" << endl;
        isNewString = true;
        pthread_cond_signal(&cvOut);
        pthread_mutex_unlock(&mtxNS);
        //sleep(1);
    }

    cout << "Will join tNS" << endl;
    pthread_join(tNS, NULL);

    pthread_mutex_destroy(&mtxNS);
    pthread_cond_destroy(&cvIn);
    pthread_cond_destroy(&cvOut);

    //cout << "Hardware concurrency is " << thread::hardware_concurrency() << endl;
    cout << "Finish..." << endl;
    return 0;
}
