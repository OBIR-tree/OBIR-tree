#ifndef PTREE_H
#define PTREE_h

#include "Node.h"
#include <algorithm>
#include <cmath>
#include <list>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <chrono>
#include <set>
#include "PathORAM.h"

#include "Enclave_u.h"
#include "sgx_urts.h"
string Com ="flag";
const int max_datasize = 1e7+5;

class Ptree :public Node{
public:
    set<Branch*> temp_stash;//负责专门存储未写回的节点数据。
    set<Branch*> stash;
    int branch_num;
    int node_num;
    int leaf_node_num;
    int data_num;
    int all_leaf_num;
    int tree_deep;
    const char *data[max_datasize];

    vector<Node*> leaf_position_map;
    vector<Branch*> position_branchs;
    map<int, string> text_position_map;
    vector<Node*> nodes;
    Node* root;
    sgx_status_t status;
    Ptree()
    {
        tree_deep = 0;
        leaf_node_num = 0;
        branch_num = 0;
        node_num = 0;
        data_num = 0;
        all_leaf_num = 0;
        ifstream dicFile;


        dicFile.open("/home/hhhead/3/output.txt");
        if (!dicFile.is_open())
        {
            printf("error!!!");
        }
        string _line;
        int _i = 0;
        while (getline(dicFile,_line))
        {
            istringstream _iss(_line);
            _iss >> dic_str[_i];
            dic_map[_i] = dic_str[_i];
            _i++;
        }
        for (int i = 0; i < dic_str.size(); i++)
        {
            Lower(dic_str[i]);
            Lower(dic_map[i]);
        }
        dicFile.close();
        ifstream inputFile;
        string name="/home/hhhead/3/10.txt";
        inputFile.open(name);
        cout << "dic" << name << endl;
        //inputFile.open("C:\\Users\\20191\\Desktop\\work\\Paper\\data\\data1\\16.txt");
        if (!inputFile.is_open())
        {
            printf("error!!!");
        }
        string line;
        int temp_ID = 0;
        while(getline(inputFile,line))
        {
            istringstream iss(line);
            Branch *mBranch = new Branch;
            mBranch->is_virtual = false;


            string data_line = to_string(branch_num);
            const char* data = data_line.c_str();
            char temp_line[200];
            status=aes_encrypt(global_eid,data,temp_line);
            if (status != SGX_SUCCESS) {
                printf("ERROR: ECall p\n");

                printf("Enter a character before exit ...\n");
                getchar();

            }
            mBranch->trueData = temp_line;
            mBranch->pointBranch = mBranch;
            mBranch->isEmpty = false;
            string text;
            double p1 = 0, p2 = 0;
            iss >> text >> p1 >> p2;
            string new_string;
            if (text.size() > 5)
            {
                new_string = text.substr(0, 5);
            }
            if (text.size() < 5)
            {
                new_string = text + string(5 - text.size(), 'X');
            }
            if (text.size() == 5)
            {
                new_string = text;
            }
            char temp_text[200];
            const char* news = new_string.c_str();
            aes_encrypt(global_eid,news,temp_text);
            mBranch->text = temp_text;
            mBranch->m_rect.min_Rec[0] = p1;
            mBranch->m_rect.min_Rec[1] = p2;
            mBranch->m_rect.max_Rec[0] = p1;
            mBranch->m_rect.max_Rec[1] = p2;
            mBranch->CalcuKeyWordRele(new_string);

            mBranch->ID = temp_ID;
            mBranch->textID = temp_ID;
            text_position_map.insert(pair<int,string>(temp_ID, temp_text));
            temp_ID++;
            position_branchs.push_back(mBranch);

            branch_num++;
            mBranch->level = 0;
        }
        data_num = branch_num;
        int deep = int(ceil(log(data_num) / log(MAX_SIZE))) + 1;
        cout << "deep" << deep << endl;
        branch_num = int(ceil((pow(MAX_SIZE, deep)) / (MAX_SIZE - 1)));
        all_leaf_num = int(pow(MAX_SIZE, deep - 1));
        cout << "branch_num" << branch_num << endl;
        cout << "all_leaf_num" << all_leaf_num << endl;
        for (int i = data_num; i < all_leaf_num; i++)
        {
            Branch* virtual_branch = new Branch();
            int virtual_ID = -1;
            char tmp_line[200];
            generate_random_block(global_eid,B - 16 - sizeof(uint32_t),tmp_line);
            string vid_str = string((const char*)(&virtual_ID), sizeof(uint32_t));
            char cipher[200];

            const char*  teml= (tmp_line + vid_str).c_str();
            aes_encrypt(global_eid,teml,cipher );
            virtual_branch->trueData = cipher;
            virtual_branch->is_virtual = true;
            virtual_branch->ID = temp_ID;
            virtual_branch->textID = temp_ID;
            char text_cipher[200];

            aes_encrypt(global_eid,Com.c_str(),text_cipher);
            virtual_branch->text = text_cipher;
            text_position_map.insert(pair<int, string>(temp_ID, Com));
            temp_ID++;
            position_branchs.push_back(virtual_branch);
        }
        inputFile.close();
        printf("The inition of the Data block is completed......");
        node_num = int(ceil(branch_num / MAX_SIZE));
        cout << "node_num" << node_num << endl;
        sort(position_branchs.begin(), position_branchs.begin() + data_num, [](Branch* brancha, Branch* branchb) {return brancha->m_rect.min_Rec[0] < branchb->m_rect.min_Rec[0]; });
        int i = 0;
        while (i < all_leaf_num)
        {
            if (MAX_SIZE + i < all_leaf_num)
            {
                sort(position_branchs.begin() + i, position_branchs.begin() + i + MAX_SIZE, [](Branch* brancha, Branch* branchb) {return brancha->m_rect.min_Rec[1] < branchb->m_rect.min_Rec[1]; });
            }
            else
            {
                sort(position_branchs.begin() + i, position_branchs.begin() + all_leaf_num, [](Branch* brancha, Branch* branchb) {return brancha->m_rect.min_Rec[1] < branchb->m_rect.min_Rec[1]; });
            }
            double min0 = 0, min1 = 0, max0 = 0, max1 = 0;
            Node* m_node = new Node;
            int index = i;
            for (index = i; index < i + MAX_SIZE && index < all_leaf_num; index++)
            {
                min0 = min(position_branchs[i]->m_rect.min_Rec[0], min0);
                min1 = min(position_branchs[i]->m_rect.min_Rec[1], min1);
                max0 = max(position_branchs[i]->m_rect.max_Rec[0], max0);
                max1 = max(position_branchs[i]->m_rect.max_Rec[1], max1);
                position_branchs[index]->curNode = m_node;
                m_node->mBranch.push_back(position_branchs[index]);
                m_node->InvertedFileUpdate(position_branchs[index]);
            }
            m_node->m_rect.max_Rec[0] = max0;
            m_node->m_rect.max_Rec[1] = max1;
            m_node->m_rect.min_Rec[0] = min0;
            m_node->m_rect.min_Rec[1] = min1;
            m_node->level = 0;
            m_node->setCount();
            m_node->ID = index / MAX_SIZE;
            nodes.push_back(m_node);
            leaf_position_map.push_back(m_node);
            leaf_node_num++;
            i = index;
        }
        int leaf_node_num = int(nodes.size());
        cout << "leaf_node_num" << leaf_node_num << endl;
        int temp_num = leaf_node_num;
        int cnt = 0;
        int _level = 1;
        while(temp_num > 1)
        {
            int temp_temp_num = int(nodes.size());

            while(cnt < temp_temp_num)
            {
                Node *m_node = new Node();
                m_node->initRectangle();
                m_node->level = _level;

                for(int i = 0; i < MAX_SIZE; i++)
                {
                    Branch *m_branch = new Branch();
                    m_branch->m_rect = nodes[cnt]->m_rect;
                    for(int j = 0; j < nodes[cnt]->mBranch.size(); j++)
                    {
                        if(!(nodes[cnt]->mBranch[j]->is_virtual))
                        {
                            m_branch->keyWeightUpdate(nodes[cnt]->mBranch[j]);

                            m_branch->textUpdate(nodes[cnt]->mBranch[j]);
                        }
                        m_branch->child.push_back(nodes[cnt]->mBranch[j]);
                        nodes[cnt]->mBranch[j]->partent = m_branch;
                    }
                    m_branch->level = _level;
                    m_branch->ID = temp_ID;
                    m_branch->textID = temp_ID;
                    text_position_map.insert(pair<int, string>(temp_ID, m_branch->text));
                    temp_ID++;
                    position_branchs.push_back(m_branch);

                    m_branch->curNode = m_node;
                    m_node->mBranch.push_back(m_branch);
                    m_node->InvertedFileUpdate(m_branch);
                    m_node->rectUpdate(m_branch);
                    nodes[cnt]->parentNode = m_node;
                    cnt++;
                }
                nodes.push_back(m_node);
            }
            temp_num /= MAX_SIZE;
            _level++;
        }
        tree_deep = _level;
        node_num = int(nodes.size());
        for(int index = leaf_node_num; index < node_num; index++)
        {
            int temp_num = int(nodes[index]->mBranch.size());
            if(temp_num < MAX_SIZE)
            {
                nodes[index]->addVirtualBranch(text_position_map,temp_ID,Com);
            }
        }
        root = nodes.back();
        printf("The procsessing of constructing a IRtree is Completed......\n");
    }


    void textPathOram(Branch *curBranch)
    {
        Node* newNode = new Node();
        int rand_num = newNode->randNum(branch_num);
        int rand_num2 = newNode->randNum(leaf_node_num);
        Node* temp_node = leaf_position_map[rand_num2];
        vector<Branch*>text_branchs;
        while(temp_node != NULL)
        {
            for (int i = 0; i < MAX_SIZE; i++)
            {
                text_branchs.push_back(temp_node->mBranch[i]);
                char out_text[200];
                string in_text = temp_node->mBranch[i]->text;
                const char* in = in_text.c_str();
                status=aes_dncrypt(global_eid,in,out_text);
                if (status != SGX_SUCCESS) {
                    printf("ERROR: ECall failed step2\n");

                    printf("Enter a character before exit ...\n");
                    getchar();

                }
                char cipher[200];
                const char* out = out_text;
                aes_encrypt(global_eid,out,cipher );
            }
            temp_node = temp_node->parentNode;
        }
        Node* curNode = curBranch->curNode;
        while (curNode != NULL)
        {
            for (int i = 0; i < MAX_SIZE; i++)
            {
                text_branchs.push_back(curNode->mBranch[i]);
                char out_text[200];
                string in_text = curNode->mBranch[i]->text;
                in_text = string("\0");
                const char* inte = in_text.c_str();
                aes_dncrypt(global_eid,inte,out_text);
                char cipher[200];
                const char* outt =out_text;
                aes_encrypt(global_eid,outt,cipher);
            }
            curNode = curNode->parentNode;
        }
        string text = text_position_map[rand_num];
        if (text.compare(Com) == 0)
        {
            curBranch->textID = rand_num;
            text_position_map[rand_num] = text_position_map[curBranch->textID];
            text_position_map[curBranch->textID] = Com;
        }

    }
    void Path_insert(Branch* mbranch, Branch* sbranch, list<kNNUtil> &Queue)
    {
        double _rele = CalcuTestSPaceRele(sbranch, mbranch);
        kNNUtil myKnn{};
        myKnn.rele = _rele;
        myKnn.pBranch = mbranch;
        if (mbranch->level == 0)
        {
            myKnn.isleaf = true;
        }
        else
        {
            myKnn.isleaf = false;
        }
        int _insert = 0;
        list<kNNUtil>::iterator pInsert = Queue.begin();
        if (Queue.size() > 0)
        {
            while (_rele < (*pInsert).rele)
            {
                pInsert++;
                if (pInsert == Queue.end())
                {
                    break;
                }
            }
        }
        Queue.emplace(pInsert, myKnn);
        return;
    }
    void Path_Search2(Rectangle rect, string& text)
    {
        vector<kNNUtil> Queue;
        Branch* search_branch = new Branch();
        textPathOram(search_branch);
        search_branch->m_rect = rect;
        search_branch->CalcuKeyWordRele(text);
        double rele = 1e5;
          for (int j = 0; j < 4; j++) {
        for (int i = 0; i < root->mBranch.size(); i++)
        {
            if (root->mBranch[i]->is_virtual == true)
            {
                continue;
            }
            kNNUtil knn_obj;
            knn_obj.pBranch = root->mBranch[i];
            textPathOram(root->mBranch[i]);
            knn_obj.rele = CalcuTestSPaceRele(root->mBranch[i], search_branch);
            if (root->mBranch[i]->level == 0)
            {
                knn_obj.isleaf = true;
            }
            else
            {
                knn_obj.isleaf = false;
            }
            if (knn_obj.rele < rele)
            {
                rele = knn_obj.rele;
            }
            Queue.push_back(knn_obj);

        }
        }
        sort(Queue.begin(), Queue.begin() + root->mBranch.size(), [](kNNUtil knna, kNNUtil knnb) {return knna.rele > knnb.rele; });
        kNNUtil ans = Queue[0];
       
       
          bool flag = false;
        while (flag != true)
        {
            vector<kNNUtil> temp_Queue;
            for (int i = 0; i < ans.pBranch->child.size(); i++)
            {
                kNNUtil knn_obj;
                knn_obj.pBranch = ans.pBranch->child[i];
                textPathOram(ans.pBranch->child[i]);
                knn_obj.rele = CalcuTestSPaceRele(ans.pBranch->child[i], search_branch);
                if (knn_obj.pBranch->is_virtual == true)
                {
                    continue;
                }
                if (knn_obj.pBranch->level == 0)
                {
                    knn_obj.isleaf = true;
                }
                else
                {
                    knn_obj.isleaf = false;
                }
                temp_Queue.push_back(knn_obj);
            }
            sort(temp_Queue.begin(), temp_Queue.begin() + ans.pBranch->child.size(), [](kNNUtil knna, kNNUtil knnb) {return knna.rele > knnb.rele; });
            ans = temp_Queue[0];
            flag = ans.isleaf;
        }
       

            for (int i = 0; i < search_K; i++) {
                pathEvcition(ans.pBranch->curNode->mBranch[i]);
                char out_line[200];
                aes_dncrypt(global_eid, ans.pBranch->curNode->mBranch[i]->trueData.c_str(), out_line);
            }
        
        return;
    }
    void Path_Search(Rectangle rect, string& text)
    {
        list<kNNUtil> Queue;
        int k = search_K;
        Branch* search_branch = new Branch();
        //textPathOram(search_branch);
        search_branch->CalcuKeyWordRele(text);
        search_branch->m_rect = rect;
        for (int i = 0; i < root->mBranch.size(); i++)
        {
            kNNUtil knn_obj;
            double _rele = root->CalcuTestSPaceRele(root->mBranch[i], search_branch);
            //textPathOram(root->mBranch[i]);
            knn_obj.rele = _rele;
            knn_obj.pBranch = root->mBranch[i];
            if (root->mBranch[i]->level == 0)
            {
                knn_obj.isleaf = true;
            }
            else
            {
                knn_obj.isleaf = false;
            }
            Queue.push_back(knn_obj);
        }
        int i = 0;
        for(auto pr = Queue.begin(); pr != Queue.end(), i < k;)
        {
            if (pr->isleaf == true)
            {
                if (pr != Queue.end())
                {
                    pr++;
                    i++;
                    continue;
                }
            }
            else
            {
                Branch* mBranch = pr->pBranch;
                for (int j = 0; j < mBranch->child.size(); i++)
                {
                    Path_insert(mBranch->child[i], search_branch, Queue);
                }
                pr = Queue.begin();
                i = 0;
            }


        }
        return;
    }


    void displayResults(vector<Datatype> results)
    {

        for (Datatype& text : results)
        {
            cout<< text << ' ';
        }
        cout << endl;

    }
    //
    void knnInsert(Branch* inBranch, list<kNNUtil>& Queue, Branch* search_branch)
    {
        int _size = inBranch->child.size();
        for (int i = 0; i < _size; i++)
        {
            Branch* myBranch = inBranch->child[i];
            double _rele = CalcuTestSPaceRele(myBranch,search_branch);
            kNNUtil myKnn{};
            myKnn.rele = _rele;
            myKnn.pBranch = myBranch;
            if (myBranch->level == 0)
            {
                myKnn.isleaf = true;
            }
            else
            {
                myKnn.isleaf = false;
            }
            int _insert = 0;
            list<kNNUtil>::iterator pInsert = Queue.begin();
            if (Queue.size() > 0)
            {
                while (_rele < (*pInsert).rele)
                {
                    pInsert++;
                    if (pInsert == Queue.end())
                    {
                        break;
                    }
                }
            }
            Queue.emplace(pInsert, myKnn);
        }
    }


    void pathRecovery(int ID1, int ID2)
    {
        Node* node1 = leaf_position_map[ID1];
        Node* node2 = leaf_position_map[ID2];
        pathR(1,node1,node2);

    }

    void searchPathBranch(Node* mNode)
    {
        Node* temp_node = mNode;
        while (temp_node != NULL)
        {
            for (int i = 0; i < MAX_SIZE; i++)
            {
                if (temp_node->mBranch[i]->is_virtual == true)
                {
                    break;
                }
                stash.insert(temp_node->mBranch[i]);
//                stash_num++;
                //temp_node->mBranch[i]->pointBranch = NULL;
            }
            insert(1,temp_node);
            if (temp_node == NULL)
            {
                return;
            }
            temp_node = temp_node->parentNode;
        }
    }
    void insertOtherPosition(Branch* mBranch)
    {
        Node* m_node = new Node();
        int rand_num = m_node->randNum(leaf_node_num);
        if (mBranch->pointBranch == NULL)
        {
            return;
        }
        string str = mBranch->pointBranch->trueData;
        Node* other_node = leaf_position_map[rand_num];
        Node* tempcurNode = mBranch->pointBranch->curNode;
        Branch* tempBranch = mBranch->pointBranch;
        mBranch->pointBranch = NULL;
        int ID1 = mBranch->curNode->ID;
        int ID2 = other_node->ID;
        searchPathBranch(other_node);
        int flag = 0;
        while (other_node->level < tree_deep)
        {
            for (int i = 0; i < MAX_SIZE; i++)
            {
                if (other_node->mBranch[i]->is_virtual == true)
                {
                    break;
                }
                if (other_node->mBranch[i]->trueData.compare(Com))
                {
                    other_node->mBranch[i]->trueData = str;
                    mBranch->pointBranch = other_node->mBranch[i];
                    mBranch->pointBranch->trueData = Com;
                    flag = 1;
                    erase(1,mBranch);
                    break;
                }

            }
            if (flag)
            {
                break;
            }
        }
        pathRecovery(ID1, ID2);
    }
    //
    void pathEvcition(Branch* mBranch)
    {
        tempstash(1,mBranch);

//        search_num++;
        Node* temp_node = mBranch->curNode;
        searchPathBranch(temp_node);
        insertOtherPosition(mBranch);
    }

    //
    //
    void branchUpdate(Branch* oBranch, Branch* nBranch)
    {
        nBranch->ID = oBranch->ID;
        nBranch->pointBranch = oBranch->pointBranch;
        nBranch->is_virtual = oBranch->is_virtual;
        nBranch->level = oBranch->level;
        nBranch->m_rect = oBranch->m_rect;
        nBranch->partent = oBranch->partent;
        nBranch->weight = oBranch->weight;
    }
    //

    chrono::nanoseconds getRunTime(double x, double y, string &text)
    {
        Rectangle rect = Rectangle();
        rect.min_Rec[0] = x;
        rect.min_Rec[1] = y;
        rect.max_Rec[0] = x;
        rect.max_Rec[1] = y;
        auto _start = chrono::high_resolution_clock::now();
//        Search(rect, text);
        Path_Search2(rect, text);
        auto _end = chrono::high_resolution_clock::now();
        auto runtime = chrono::duration_cast<chrono::nanoseconds>(_end - _start);
        double total_time = double(runtime.count()) * chrono::nanoseconds::period::num / (chrono::nanoseconds::period::den);
        cout << "run_time " << total_time << endl;
        return runtime;

    }

};

#endif
