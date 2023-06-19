#include "creattest.h"
#include "alist.h"
#include "btree.h"
#include "treat.h"
#include "deadline.h"
#include "registry.h"
#include "queue.h"
#include "centralqueue.h"
#include "BPlus_tree.cpp"
//#include "BPlus_node.cpp"
#include <stdlib.h>
#include <stdio.h>

#include <cstdlib>
#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <sstream>

using namespace std;

void query_by_age(int age, BTree<int, Registration *> *B);
int main()

{
    int month_register_count = 0;
    int month_wait_count = 0;
    int month_withdraw_count = 0;
    int month_appoint_count = 0;
    int people_treat_count = 0;
    int type_total = 0;
    int total_num = 0;
    // create folders
    string folderpath = "./patients";
    string command;
    command = "mkdir -p " + folderpath;
    system(command.c_str());

    folderpath = "./weeklyreport";
    command = "mkdir -p " + folderpath;
    system(command.c_str());

    folderpath = "./monthlyreport";
    command = "mkdir -p " + folderpath;
    system(command.c_str());

    folderpath = "./withdraw";
    command = "mkdir -p" + folderpath;
    system(command.c_str());

    folderpath = "./query_result";
    command = "mkdir -p" + folderpath;
    system(command.c_str());
    // we assume that our program start at 2022.01.01
    int year = 2022;
    cout << "Simulate from 2022.01.01 till 2022.03.03" << endl;
    int months = 3;
    /*cout << "please tape the month needed to test" << endl;

    while (true) {
        cin >> months;
        if (months > 12 || months < 1) {
            cout << "error input of month" << endl;
            cout << "please tape the month needed to test" << endl;
        }
        else {
            break;
        }
    }*/
    // set max daily registration
    int daily_max = 8;
    /*cout << "please tape the maximum possible number of regestrations" << endl;
    while (true) {
        cin >> daily_max;
        if (daily_max * months > 48) {
            cout << "Sorry, number too large!" << endl;
            cout << "please tape the maximum possible number of regestrations" << endl;
        }
        else {
            break;
        }
    }*/

    // determine how to order the weekly reports
    int ordertype;
    while (true) {
        cout << "Please choose how to order the report" << endl;
        cout << " 0 for name, 1 for profession category or 2 for age group" << endl;
        cin >> ordertype;
        if (ordertype != 0 && ordertype != 1 && ordertype != 2) {
            cout << "Please choose the right type! 0, 1 or 2" << endl;
        }
        else {
            break;
        }
    }

    // creat test cases and queues
    creattest(months, daily_max);
    // now allow the user to modify the csv

    cout
        << "Now you can modify the information of patients of add new patient in corresponding csv file in folder \"patients\"."
        << endl;
    cout << "Please pay attention to the format." << endl;
    cout << "Type c to continue..." << endl;
    while (true) {
        int halt = 0;
        cin >> halt;
        if (true) {
            break;
        }
    }

    hashmap<int, Registration *> hashtab;
    hashmap<int, CentralQueueNode *> hashtabcen;

    // initialization
    Registration *wait_list[10000];
    for (int i = 0; i < 10000; i++) {
        wait_list[i] = NULL;
    }
    CentralQueueNode *central_node0 = NULL;
    CentralQueueNode *central_node1 = NULL;
    CentralQueueNode *central_node2 = NULL;
    Registration *people_treated[200 * 7];
    for (int i = 0; i < 200 * 7; i++) {
        people_treated[i] = NULL;
    }

    CBPlusTree<int, Registration *> *Bp_tree = new CBPlusTree<int, Registration *>;
    BTree<int, Registration *> *B_tree = new BTree<int, Registration *>(10);
    Local_Q *localque[225];  // the array of pointer to queue// the array of identifier for each registry
    for (int i = 0; i < 225; i++) {
        localque[i] = new Local_Q(i);
    }

    Deadline *ddl_list[10000];
    for (int i = 0; i < 10000; i++) {
        ddl_list[i] = NULL;
    }

    // creat treat place
    float ad1[2];
    ad1[0] = 250.0;
    ad1[1] = 250.0;
    float ad2[2];
    ad2[0] = 250.0;
    ad2[1] = 750.0;
    float ad3[2];
    ad3[0] = 750.0;
    ad3[1] = 250.0;
    float ad4[2];
    ad4[0] = 750.0;
    ad4[1] = 750.0;
    TreatLoc *treatloc1 = new TreatLoc(ad1);
    TreatLoc *treatloc2 = new TreatLoc(ad2);
    TreatLoc *treatloc3 = new TreatLoc(ad3);
    TreatLoc *treatloc4 = new TreatLoc(ad4);

    // we use for loop to simulate the time passing by
    for (int m = 1; m <= months; m++) {  // m is the month to loop

        for (int d = 1; d <= 30; d++)  // d is the day
        {
            for (int r = 0; r < 2; r++)  // two round in one day
            {
                // read the testcase file in this half of the day
                int filename = m * 1000 + d * 10 + r;
                string sfilename = "patients/" + std::to_string(filename) + ".csv";
                ifstream infile(sfilename, ios::in);
                // infile.open;
                if (!infile) {
                    cout << "fail to open1" << endl;
                    exit(1);
                }

                // read the file of registration
                string line;
                getline(infile, line);

                while (getline(infile, line)) {
                    string field;
                    istringstream sin(line);

                    // identifier for registry
                    getline(sin, field, ',');
                    int identifier = atoi(field.c_str());

                    // stamp time
                    getline(sin, field, ',');
                    int reg_year = atoi(field.c_str());
                    getline(sin, field, ',');
                    int reg_month = atoi(field.c_str());
                    getline(sin, field, ',');
                    int reg_day = atoi(field.c_str());
                    getline(sin, field, ',');
                    int reg_hour = atoi(field.c_str());
                    getline(sin, field, ',');
                    int reg_minite = atoi(field.c_str());
                    Time *timestamp = new Time(reg_year, reg_month, reg_day, reg_hour, reg_minite);
                    // personal information
                    getline(sin, field, ',');
                    string name = field;
                    getline(sin, field, ',');
                    string phone = field;
                    getline(sin, field, ',');
                    string wechat = field;
                    getline(sin, field, ',');
                    string email = field;

                    getline(sin, field, ',');
                    int identification = atoi(field.c_str());
                    getline(sin, field, ',');
                    float x = atof(field.c_str());
                    getline(sin, field, ',');
                    float y = atof(field.c_str());
                    float addr[2];
                    addr[0] = x;
                    addr[1] = y;

                    getline(sin, field, ',');
                    int profession = atoi(field.c_str());
                    getline(sin, field, ',');
                    int risk = atoi(field.c_str());
                    getline(sin, field, ',');
                    int birth_year = atoi(field.c_str());
                    getline(sin, field, ',');
                    int birth_month = atoi(field.c_str());
                    getline(sin, field, ',');
                    int birth_day = atoi(field.c_str());
                    Time *birth = new Time(birth_year, birth_month, birth_day);

                    getline(sin, field, ',');
                    int dead_year = atoi(field.c_str());
                    // cout << "1" << endl;
                    getline(sin, field, ',');
                    int dead_month = atoi(field.c_str());
                    getline(sin, field, ',');
                    int dead_day = atoi(field.c_str());
                    Time *deadline = new Time(dead_year, dead_month, dead_day);
                    getline(sin, field, ',');
                    int type = atoi(field.c_str());
                    // add to local queue
                    localque[identifier]->add(identification, name, addr, phone, wechat, email, profession, birth, risk,
                                              deadline, timestamp, identifier, type);  // a new reg created and in queue
                    // cout << "1" << endl;
                    Registration *B = new Registration(identification, name, addr, phone, wechat, email, profession,
                                                       birth, risk, deadline, timestamp, type);
                    delete timestamp;
                    delete birth;
                    delete deadline;
                    B->set_id(localque[identifier]->get_numitems() - 1, identifier);  // for judgement
                    month_register_count++;

                    // INSERT IT INTO THE B+ TREE
                    bool success = Bp_tree->insert(B->get_identification(), B);
                    // insert it into B tree, using year of birth as key
                    B_tree->insert(B->get_birth(), B);

                    // determine if the new registration is in the hash, if so , it's a update of information
                    if (!success &&
                        hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())->treated != 1) {
                        cout << " update " << endl;
                        int old_risk = hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())
                                           ->get_risk();  // risk before update
                        int old_profession =
                            hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())
                                ->get_profession();
                        hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())
                            ->set_risk(B->get_risk());  // update
                        int new_risk = hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())
                                           ->get_risk();  // risk after update

                        if (B->get_profession() > old_profession)  // only update when the priority increases
                        {
                            hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())
                                ->set_profession(B->get_profession());
                            cout << "Someone updated his profession" << endl;
                        }
                        if (new_risk != old_risk) {
                            cout << "Someone updated his risk status" << endl;
                            if (hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())->place ==
                                1)  // was in wait list
                            {
                                if (hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())
                                            ->get_risk() == 2 &&
                                    hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())
                                            ->withdrawn == 0) {
                                    hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())
                                        ->waittime = 29;
                                }
                                else if (new_risk < 2 ||
                                         hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())
                                                 ->withdrawn == 1) {
                                    hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())
                                        ->waittime = 13;
                                }
                                else {
                                    hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())
                                        ->waittime = 0;  // leave wait list
                                }
                            }
                            else if (hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())
                                         ->place == 2)  // was already in central queue
                            {
                                if (new_risk == 2 &&
                                    hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())
                                            ->withdrawn == 0) {
                                    hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())->place =
                                        1;  // to wait list
                                    hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())
                                        ->waittime = 29;
                                    if (hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())
                                            ->type == 0) {
                                        central_node0 = central_node0->remove_node(
                                            hashtabcen.retrieve((*(Bp_tree->get_data(B->get_identification())))
                                                                    ->get_id()));  // remove it from central node
                                    }
                                    if (hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())
                                            ->type == 1) {
                                        central_node1 = central_node1->remove_node(
                                            hashtabcen.retrieve((*(Bp_tree->get_data(B->get_identification())))
                                                                    ->get_id()));  // remove it from central node
                                    }
                                    if (hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())
                                            ->type == 2) {
                                        central_node2 = central_node2->remove_node(
                                            hashtabcen.retrieve((*(Bp_tree->get_data(B->get_identification())))
                                                                    ->get_id()));  // remove it from central node
                                    }

                                    for (int i_wait = 0; i_wait < 10000; i_wait++) {
                                        if (wait_list[i_wait] == NULL) {
                                            wait_list[i_wait] =
                                                hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))
                                                                     ->get_id());  // define wait_list in main function
                                            break;
                                        }
                                        if (i_wait == 10000 && (wait_list[i_wait] != NULL)) {
                                            cout << "error: more than 10000 people on the wait list";
                                        }
                                    }
                                }
                                else if (new_risk < 2 &&
                                         hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())
                                                 ->withdrawn == 1) {
                                    // similar as above
                                    hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())->place =
                                        1;
                                    hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())
                                        ->waittime = 13;
                                    if (hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())
                                            ->type == 0) {
                                        central_node0 = central_node0->remove_node(
                                            hashtabcen.retrieve((*(Bp_tree->get_data(B->get_identification())))
                                                                    ->get_id()));  // remove it from central node
                                    }
                                    if (hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())
                                            ->type == 1) {
                                        central_node1 = central_node1->remove_node(
                                            hashtabcen.retrieve((*(Bp_tree->get_data(B->get_identification())))
                                                                    ->get_id()));  // remove it from central node
                                    }
                                    if (hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())
                                            ->type == 2) {
                                        central_node2 = central_node2->remove_node(
                                            hashtabcen.retrieve((*(Bp_tree->get_data(B->get_identification())))
                                                                    ->get_id()));  // remove it from central node
                                    }
                                    for (int i_wait = 0; i_wait < 10000; i_wait++) {
                                        if (wait_list[i_wait] == NULL) {
                                            wait_list[i_wait] =
                                                hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))
                                                                     ->get_id());  // define wait_list in main function
                                            break;
                                        }
                                        if (i_wait == 10000 && (wait_list[i_wait] != NULL)) {
                                            cout << "error: more than 10000 people on the wait list";
                                        }
                                    }
                                }
                                else  // still in central queue
                                {
                                    CentralQueueNode *node = new CentralQueueNode(B);  // add it to central queue
                                    if (hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())
                                            ->type == 0) {
                                        central_node0 = central_node0->remove_node(
                                            hashtabcen.retrieve((*(Bp_tree->get_data(B->get_identification())))
                                                                    ->get_id()));  // remove it from central node

                                        if (central_node0 == NULL) {  // cout << "31" << endl;

                                            central_node0 = node;
                                            node = NULL;
                                        }
                                        else {
                                            // cout << "41" << endl;
                                            central_node0 = central_node0->insert(node);
                                        }
                                    }
                                    if (hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())
                                            ->type == 1) {
                                        central_node1 = central_node1->remove_node(
                                            hashtabcen.retrieve((*(Bp_tree->get_data(B->get_identification())))
                                                                    ->get_id()));  // remove it from central node
                                        if (central_node1 == NULL) {               // cout << "31" << endl;

                                            central_node1 = node;
                                            node = NULL;
                                        }
                                        else {
                                            // cout << "41" << endl;
                                            central_node1 = central_node1->insert(node);
                                        }
                                    }
                                    if (hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())
                                            ->type == 2) {
                                        central_node2 = central_node2->remove_node(
                                            hashtabcen.retrieve((*(Bp_tree->get_data(B->get_identification())))
                                                                    ->get_id()));  // remove it from central node
                                        if (central_node2 == NULL) {               // cout << "31" << endl;
                                            central_node2 = node;
                                            node = NULL;
                                        }
                                        else {
                                            // cout << "41" << endl;
                                            central_node2 = central_node2->insert(node);
                                        }
                                    }
                                }
                            }
                        }
                        else {
                            CentralQueueNode *node = new CentralQueueNode(B);  // add it to central queue
                            if (hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())->type ==
                                0) {
                                central_node0 = central_node0->remove_node(
                                    hashtabcen.retrieve((*(Bp_tree->get_data(B->get_identification())))
                                                            ->get_id()));  // remove it from central node

                                if (central_node0 == NULL) {  // cout << "31" << endl;

                                    central_node0 = node;
                                    node = NULL;
                                }
                                else {
                                    // cout << "41" << endl;
                                    central_node0 = central_node0->insert(node);
                                }
                            }
                            if (hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())->type ==
                                1) {
                                central_node1 = central_node1->remove_node(
                                    hashtabcen.retrieve((*(Bp_tree->get_data(B->get_identification())))
                                                            ->get_id()));  // remove it from central node
                                if (central_node1 == NULL) {               // cout << "31" << endl;

                                    central_node1 = node;
                                    node = NULL;
                                }
                                else {
                                    // cout << "41" << endl;
                                    central_node1 = central_node1->insert(node);
                                }
                            }
                            if (hashtab.retrieve((*(Bp_tree->get_data(B->get_identification())))->get_id())->type ==
                                2) {
                                central_node2 = central_node2->remove_node(
                                    hashtabcen.retrieve((*(Bp_tree->get_data(B->get_identification())))
                                                            ->get_id()));  // remove it from central node
                                if (central_node2 == NULL) {               // cout << "31" << endl;

                                    central_node2 = node;
                                    node = NULL;
                                }
                                else {
                                    // cout << "41" << endl;
                                    central_node2 = central_node2->insert(node);
                                }
                            }
                        }
                    }
                    else {
                        hashtab.add(B->get_id(), B);
                    }
                    // cout << " success update" << endl;
                    // put it into the ddl_list if it has a DEADLINE
                    /*     if (dead_year != 0)
                                       {   int dead_index = (dead_month-1)*30+dead_day - 1 ;
                                           if (ddl_list[dead_index] == NULL)
                                           {
                                               Deadline* A = new Deadline();
                                               // Registration* B = new Registration(identification, name, addr, phone,
                       wechat, email,
                                               //        profession, birth, risk, deadline, timestamp);
                                               cout << "success new deadline" << endl;
                                               A->add(B, identifier);
                                               cout << "success new add1" << endl;
                                           //    ddl_list[dead_index] = A;

                                           }
                                           else
                                           {
                                               // Registration* B = new Registration(identification, name, addr, phone,
                       wechat, email,
                                               //                    profession, birth, risk, deadline, timestamp);
                                               cout << "success new add1" << endl;
                                              ddl_list[dead_index]->add(B,identifier);
                                           }

                                       }                  */////////////////////////////////////////////////////////////////////////

                    // cout << "ready to delete" << endl;
                    // delete B;
                }
                infile.close();  // finished reading the file and building the local queue
                // cout << "success reading file 1" << endl;
                // read the file of withdraw

                string sfilename1 = "withdraw/" + std::to_string(filename) + ".csv";
                ofstream wdfile;
                wdfile.open(sfilename1);
                wdfile << "Indentification" << endl;
                int identification;
                int n = rand() % 10;
                for (int i = 0; i <= n; i++) {
                    identification = randnum(10000000, 99999999);
                    wdfile << identification << endl;
                }
                wdfile.close();

                ifstream infile1(sfilename1, ios::in);
                // infile1.open;
                if (!infile1) {
                    cout << "no withdraw today" << endl;
                }
                string line1;
                getline(infile1, line1);
                while (getline(infile1, line1)) {
                    int id_withdraw = atoi(line1.c_str());
                    if (hashtab.member(id_withdraw)) {
                        hashtab.retrieve(id_withdraw)->withdrawn = 2;
                        month_withdraw_count++;
                    }
                    else {
                        // cout << "id does not existed" << endl;
                    }
                }
                infile1.close();

                // cout << "success reading file withdraw" << endl;

                // put element in waitlist in centralized queue
                for (int check_wait = 0; check_wait < 10000; check_wait++) {
                    if (wait_list[check_wait] != NULL)  // find a place in wait_list the is not empty
                    {
                        if (wait_list[check_wait]->treated == 1)  // the one in waited list is treated
                        {
                            wait_list[check_wait] = NULL;  // remove him
                            continue;
                        }
                        else {
                            if (wait_list[check_wait]->waittime ==
                                0)  // if he is not treated and the waiting time is up
                            {
                                CentralQueueNode *node =
                                    new CentralQueueNode(wait_list[check_wait]);  // add it to central queue
                                wait_list[check_wait]->place = 2;

                                // to determine which cental queue to get through
                                if (wait_list[check_wait]->type == 0) {
                                    if (central_node0 == NULL) {
                                        central_node0 = node;
                                    }
                                    else {
                                        central_node0 = central_node0->insert(node);
                                    }
                                }
                                if (wait_list[check_wait]->type == 1) {
                                    if (central_node1 == NULL) {
                                        central_node1 = node;
                                    }
                                    else {
                                        central_node1 = central_node1->insert(node);
                                    }
                                }
                                if (wait_list[check_wait]->type == 2) {
                                    if (central_node2 == NULL) {
                                        central_node2 = node;
                                    }
                                    else {
                                        central_node2 = central_node2->insert(node);
                                    }
                                }

                                node = NULL;
                                wait_list[check_wait] = NULL;
                            }
                            else {
                                wait_list[check_wait]->waittime =
                                    wait_list[check_wait]->waittime - 1;  // if the time is not up, delete it
                            }
                        }
                    }
                }
                // cout << " success in put wait list into central queue" << endl;

                for (int i = 0; i < 225; i++) {
                    Local_Q *copy_lq = localque[i];
                    while (copy_lq->get_numitems() > 0) {
                        Registration *copy_reg = copy_lq->q->front();
                        // pop the local queue to waiting list
                        if ((copy_reg->get_risk() == 2) || (copy_reg->withdrawn == 1 && copy_reg->get_risk() < 2)) {
                            copy_reg->place = 1;
                            for (int i_wait = 0; i_wait < 10000; i_wait++) {
                                if (wait_list[i_wait] == NULL) {
                                    wait_list[i_wait] = copy_reg;  // define wait_list in main function
                                    break;
                                }
                                if (i_wait == 10000 && (wait_list[i_wait] != NULL)) {
                                    cout << "error: more than 10000 people on the wait list";
                                    return 0;
                                }
                            }
                            if (copy_reg->get_risk() == 2) {
                                copy_reg->waittime = 29;
                            }
                            else {
                                copy_reg->waittime = 13;
                            }
                            // cout << "Someone go to the waiting list" << endl;
                        }
                        // pop the local queue to central queue
                        else {  // pop it to central queue
                            copy_reg->place = 2;
                            CentralQueueNode *node = new CentralQueueNode(copy_reg);
                            if (copy_reg->type == 0) {
                                if (central_node0 == NULL) {
                                    central_node0 = node;
                                    node = NULL;
                                    hashtabcen.add(copy_reg->get_id(), central_node0);
                                }
                                else {
                                    central_node0 = central_node0->insert(node);
                                    hashtabcen.add(copy_reg->get_id(), node);
                                    node = NULL;
                                }
                            }
                            if (copy_reg->type == 1) {
                                if (central_node1 == NULL) {
                                    central_node1 = node;
                                    node = NULL;
                                    hashtabcen.add(copy_reg->get_id(), central_node1);
                                }
                                else {
                                    central_node1 = central_node1->insert(node);
                                    hashtabcen.add(copy_reg->get_id(), node);
                                    node = NULL;
                                }
                            }
                            if (copy_reg->type == 2) {
                                if (central_node2 == NULL) {
                                    central_node2 = node;
                                    node = NULL;
                                    hashtabcen.add(copy_reg->get_id(), central_node2);
                                }
                                else {
                                    central_node2 = central_node2->insert(node);
                                    hashtabcen.add(copy_reg->get_id(), node);
                                    node = NULL;
                                }
                            }
                        }
                        // cout << "someone go to cental queue" << endl;
                        // push every patient to the hashtable

                        hashtab.add(copy_reg->get_id(), copy_reg);
                        copy_lq->q->pop();
                        copy_lq->set_numitems(copy_lq->get_numitems() - 1);

                        copy_reg = NULL;

                        // cout << "local queue to hash" << endl;
                    }
                }

                // cout << "success out" << endl;  // creat treat locations

                /*
                                            int patient_today;
                                            int withdrawn_in_deadline=0;
                                                //the address of location of treatment
                                                // put people with ddl into the treated list
                                            if (ddl_list[(m-1)*30 + d-1] != NULL)
                                                {
                                                    for (int i = 0; i < ddl_list[(m-1)*30+d-1]->numitem ; i++)
                                                    {   cout << "success 2" << endl;

                                                    cout << ddl_list[(m-1)*30+d-1]->q->front() << endl;

                                                    cout << " sssssssssssssss" << endl;

                                                        if(ddl_list[(m-1)*30+d-1]->q->front()->withdrawn != 2 &&
                   ddl_list[(m-1)*30+d-1]->q->front()->treated !=1 ) {   cout << "success 1" << endl; Time* t = new
                   Time(year, m , d); ddl_list[(m-1)*30+d-1]->q->front()->treat_time= t;
                                                            ddl_list[(m-1)*30+d-1]->q->front()->treated = 1;
                                                             for (int i = 0; i < 200*7; i++)
                                                            {   cout << "success 3" << endl;
                                                                if (people_treated[i]==NULL)
                                                                {
                                                                    people_treat_count ++ ;
                                                                    people_treated[i]=
                   ddl_list[(m-1)*30+d-1]->q->front();

                                                                    break;
                                                                }
                                                            }
                                                            cout << "success put in people treated" << endl;


                                                            float distt[4];
                                                            distt[0] =
                   (ddl_list[(m-1)*30+d-1]->q->front()->get_addr()[0]-treatloc1->addr[0])*(ddl_list[(m-1)*30+d-1]->q->front()->get_addr()[0]-treatloc1->addr[0])+(ddl_list[(m-1)*30+d-1]->q->front()->get_addr()[1]-treatloc1->addr[1])*(ddl_list[(m-1)*30+d-1]->q->front()->get_addr()[1]-treatloc1->addr[1]);
                                                            distt[1] =
                   (ddl_list[(m-1)*30+d-1]->q->front()->get_addr()[0]-treatloc2->addr[0])*(ddl_list[(m-1)*30+d-1]->q->front()->get_addr()[0]-treatloc2->addr[0])+(ddl_list[(m-1)*30+d-1]->q->front()->get_addr()[1]-treatloc2->addr[1])*(ddl_list[(m-1)*30+d-1]->q->front()->get_addr()[1]-treatloc2->addr[1]);
                                                            distt[2] =
                   (ddl_list[(m-1)*30+d-1]->q->front()->get_addr()[0]-treatloc3->addr[0])*(ddl_list[(m-1)*30+d-1]->q->front()->get_addr()[0]-treatloc3->addr[0])+(ddl_list[(m-1)*30+d-1]->q->front()->get_addr()[1]-treatloc3->addr[1])*(ddl_list[(m-1)*30+d-1]->q->front()->get_addr()[1]-treatloc3->addr[1]);
                                                            distt[3] =
                   (ddl_list[(m-1)*30+d-1]->q->front()->get_addr()[0]-treatloc4->addr[0])*(ddl_list[(m-1)*30+d-1]->q->front()->get_addr()[0]-treatloc4->addr[0])+(ddl_list[(m-1)*30+d-1]->q->front()->get_addr()[1]-treatloc4->addr[1])*(ddl_list[(m-1)*30+d-1]->q->front()->get_addr()[1]-treatloc4->addr[1]);
                                                            TreatLoc* loc[4];
                                                            loc[0] = treatloc1;
                                                            loc[1] = treatloc2;
                                                            loc[2] = treatloc3;
                                                            loc[3] = treatloc4;
                                                            int closest=0;
                                                            for(int k = 0; k < 4; k++)
                                                            {
                                                                if(loc[k]->occupied < 50)
                                                                {        //find a center that is not full
                                                                    closest = k;
                                                                    break;
                                                                }
                                                            }

                                                            for( int locs = 0; locs < 4; locs++)
                                                            {
                                                                if ((distt[locs] < distt[closest]) &&
                   (loc[locs]->occupied < 50))
                                                                {
                                                                    closest = locs;
                                                                }
                                                            }
                                                            loc[closest]->occupied++;
                                                            // regist the patient to the list of the treat location
                                                            loc[closest]->patient[(loc[closest]->occupied)-1] =
                   ddl_list[(m-1)*30+d-1]->q->front();
                                                            // put it into the treated list of people
                                                             ddl_list[(m-1)*30+d-1]->q->pop();
                                                        }else
                                                        {
                                                            withdrawn_in_deadline ++;
                                                        }


                                                    }
                                                    cout << "success 2d" << endl;
                                                    patient_today = ddl_list[ (m-1)*30 + d-1]->numitem -
                   withdrawn_in_deadline; //the people with deadline today ddl_list[(m-1)*30+d-1]->numitem = 0; delete
                   ddl_list[(m-1)*30+d-1];
                                                }

                                                 cout << "treat the one with ddl" << endl;




                                            if (patient_today >= 200)
                                                {
                                                    cout << "too many people have ddl today (more than 200)" << endl;
                                                }            */
                int num = 0;
                for (int count = 200; count > 0; count--) {
                    CentralQueueNode *tempo;
                    CentralQueueNode *tempos;
                    // two copies
                    if (num % 3 == 0) {
                        tempo = central_node0;
                        tempos = central_node0;
                        // cout << "type zero" << endl;
                    }
                    if (num % 3 == 1) {
                        tempo = central_node1;
                        tempos = central_node1;
                        // cout << "type one" << endl;
                    }
                    if (num % 3 == 2) {
                        tempo = central_node2;
                        tempos = central_node2;
                    }

                    if (tempos == NULL) {
                        break;
                    }

                    while (tempo->registration->treated == 1 ||
                           tempo->registration->withdrawn == 2) {  // if the patient is already treated or withdrawn
                        tempos = tempos->remove_node(tempos);

                        if (tempos == NULL) {
                            num++;
                            break;
                        }
                        tempo = tempos;
                    }

                    tempo->registration->treat_time->year = year;
                    tempo->registration->treat_time->month = m;
                    tempo->registration->treat_time->day = d;

                    float dist[4];
                    dist[0] = (tempo->registration->get_addr()[0] - treatloc1->addr[0]) *
                                  (tempo->registration->get_addr()[0] - treatloc1->addr[0]) +
                              (tempo->registration->get_addr()[1] - treatloc1->addr[1]) *
                                  (tempo->registration->get_addr()[1] - treatloc1->addr[1]);
                    dist[1] = (tempo->registration->get_addr()[0] - treatloc2->addr[0]) *
                                  (tempo->registration->get_addr()[0] - treatloc2->addr[0]) +
                              (tempo->registration->get_addr()[1] - treatloc2->addr[1]) *
                                  (tempo->registration->get_addr()[1] - treatloc2->addr[1]);
                    dist[2] = (tempo->registration->get_addr()[0] - treatloc3->addr[0]) *
                                  (tempo->registration->get_addr()[0] - treatloc3->addr[0]) +
                              (tempo->registration->get_addr()[1] - treatloc3->addr[1]) *
                                  (tempo->registration->get_addr()[1] - treatloc3->addr[1]);
                    dist[3] = (tempo->registration->get_addr()[0] - treatloc4->addr[0]) *
                                  (tempo->registration->get_addr()[0] - treatloc4->addr[0]) +
                              (tempo->registration->get_addr()[1] - treatloc4->addr[1]) *
                                  (tempo->registration->get_addr()[1] - treatloc4->addr[1]);
                    TreatLoc *loc[4];
                    loc[0] = treatloc1;
                    loc[1] = treatloc2;
                    loc[2] = treatloc3;
                    loc[3] = treatloc4;
                    int closest = 0;
                    for (int k = 0; k < 4; k++) {
                        if (loc[k]->occupied < 50) {  // find a center that is not full
                            closest = k;
                            break;
                        }
                    }

                    for (int locs = 0; locs < 4; locs++) {
                        if ((dist[locs] < dist[closest]) && (loc[locs]->occupied < 50)) {
                            closest = locs;
                        }
                    }
                    loc[closest]->occupied++;
                    // regist the patient to the list of the treat location
                    loc[closest]->patient[(loc[closest]->occupied) - 1] = tempo->registration;

                    month_appoint_count++;
                    tempo->registration->treated = 1;
                    tempo->registration->treat_time->year = year;
                    tempo->registration->treat_time->month = m;
                    tempo->registration->treat_time->day = d;

                    for (int i = 0; i < 200 * 7; i++) {
                        if (people_treated[i] == NULL) {
                            people_treat_count++;
                            people_treated[i] = tempo->registration;
                            break;
                        }
                    }
                    if (tempos != NULL) {
                        tempos = tempos->remove_node(tempos);
                    }

                    if (num % 3 == 0) {
                        central_node0 = tempos;
                    }
                    if (num % 3 == 1) {
                        central_node1 = tempos;
                    }
                    if (num % 3 == 2) {
                        central_node2 = tempos;
                    }

                    num++;
                    tempo = NULL;
                    tempos = NULL;
                }

                cout << m << "," << d << "," << r << endl;

            }  // end of half a day
            if (((m - 1) * 30 + d) % 7 == 0) {
                cout << "A week past!" << endl;
                hashtab.week_report(((m - 1) * 30 + d) / 7, people_treated, people_treat_count, ordertype);
                // empty the list each week
                for (int i = 0; i < 200 * 7; i++) {
                    people_treat_count = 0;
                    people_treated[i] = NULL;
                }
            }

            if (d == 30) {
                cout << "A month past!" << endl;
                hashtab.month_report(m, month_register_count, month_wait_count, month_appoint_count,
                                     month_withdraw_count, d);
                for (int c = 0; c < 10000; c++) {
                    if (wait_list[c] != NULL) {
                        month_wait_count++;
                    }
                }
            }

        }  // day
        month_register_count = 0;
        month_withdraw_count = 0;
        month_appoint_count = 0;
        month_wait_count = 0;
    }  // month
    // Bp_tree->print();
    cout << "Simulation done." << endl;
    cout << "Patients's identification number is sorted in ages, stored in folder \"query_result\"" << endl;
    cout << "Tree form in \"B+ Tree.csv\"" << endl;
    cout << "BTree is printed on screen" << endl;
    Bp_tree->print();
    B_tree->print();
    /* int query_age;
     while (true) {
         cin >> query_age;
         if (query_age < 0 || query_age > 111) {
             cout << "Illegal age!";
         }
         else {
             break;
         }
     }
     if (query_age == 111) {
         for (int i = 10; i < 111; i++) {
             query_by_age(i, B_tree);
         }
     }
     else {
         query_by_age(query_age, B_tree);
     }*/
    for (int i = 1; i < 111; i++) {
        query_by_age(i, B_tree);
    }

}  // main

void query_by_age(int age, BTree<int, Registration *> *B)
{
    int year = 2022 - age;
    AList<Registration *> *result_list = B->find(year);
    if (result_list == 0) {
        return;
    }
    int total = result_list->get_num();
    string name = "query_result/" + to_string(age) + ".csv";
    ofstream result;
    result.open(name);
    result << "All the identification of patients at age " << to_string(age) << "." << endl;
    result << total << " patient in total." << endl;
    for (int i = 0; i < total; i++) {
        result << result_list->getitem(i)->get_identification() << endl;
    }
    result.close();
}
