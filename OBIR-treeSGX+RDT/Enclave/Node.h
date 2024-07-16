#ifndef NODE_H
#define NODE_H

#include "PathORAM.h"
#include "Branch.h"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <iostream>
#include <map>

using namespace std;

class Node
{

public:
    int level;
    int count;
    vector<InvertedFile> child_iFile;
    vector<Branch*> mBranch;
    Rectangle m_rect;
    int ID;
    Node* parentNode;
    Node()
    {
        ID = 0;
        level = -1;
        count = 0;
        for (int i = 0; i < 2; i++)
        {
            m_rect.max_Rec[i] = 0;
            m_rect.min_Rec[i] = 0;
        }
        parentNode = NULL;
    }

    static int randNum(int n)
    {
        return uniform_int_distribution<int>(0,n-1)(gen);
    }

    bool IsInternalNode()
    {
        return (level > 0);
    }

    bool IsLeaf()
    {
        return (level == 0);
    }

    void SetLevel()
    {
        if (count != 0)
        {
            this->level = this->mBranch[0]->level;
        }
    }

    void setCount()
    {
        int _count = 0;
        for (int i = 0; i < MAX_SIZE; i++)
        {
            if (this->mBranch[i]->is_virtual != true)
            {
                _count++;
            }
            else
            {
                break;
            }
        }
        this->count = _count;
    }

    void invertedFileInit()
    {
        for(int index = 0; index < this->mBranch.size(); index++)
        {
            InvertedFile *tempfile = (InvertedFile*)malloc(sizeof(InvertedFile));
            for(int t = 0; t < this->mBranch[index]->weight.size(); t++)
            {
                Keyword _key(dic_map[t], this->mBranch[index]->weight[t]);

                if (tempfile)
                {
                    tempfile->words.push_back(_key);
                }
            }
            if (tempfile != NULL)
            {
                tempfile->index = index + 1;
                this->child_iFile.push_back(*tempfile);
            }

        }
    }

    void InvertedFileUpdate(Branch *nBranch)
    {
        InvertedFile* tempfile{};
        for(int index = 0; index < nBranch->weight.size(); index++)
        {
            Keyword _key(dic_map[index], nBranch->weight[index]);
            if (tempfile)
            {
                tempfile->words.push_back(_key);
            }
        }
        if (tempfile != NULL)
        {
            tempfile->index = this->count + 1;
            this->child_iFile.push_back(*tempfile);
        }
    }

    void InvertedFilePrint()
    {
        for(int index = 0; index < this->child_iFile.size(); index++)
        {
            printf("%d:",this->child_iFile[index].index);
            for(Keyword temp : this->child_iFile[index].words)
            {
                if(temp.weight != 0)
                {
                    //printf("<%s,%lf>\t",temp.name,temp.weight);
                }
            }
        }
    }
    void rectUpdate(Branch *m_branch)
    {
        for(int index = 0; index < 2; index++)
        {
            this->m_rect.min_Rec[index] = min(this->m_rect.min_Rec[index], m_branch->m_rect.min_Rec[index]);
            this->m_rect.max_Rec[index] = max(this->m_rect.max_Rec[index], m_branch->m_rect.max_Rec[index]);
        }
    }

    void initRectangle()
    {
        for(int index = 0; index < 2; index++)
        {
            this->m_rect.min_Rec[index] = inf;
            this->m_rect.max_Rec[index] = -1;
        }
    }
    void addVirtualBranch(byte *_key,map<int,string> text_position_map,int temp_ID,string Com)
    {
        for(int index = int(this->mBranch.size()); index < MAX_SIZE; index++)
        {
            this->setCount();
            Branch* virtual_branch = new Branch();
            int virtual_ID = -1;
            string tmp_line = PathORAM::generate_random_block(B - 16 - sizeof(uint32_t));
            string vid_str = string((const char*)(&virtual_ID), sizeof(uint32_t));
            string cipher;
            cipher=aes_encrypt(tmp_line + vid_str;
            virtual_branch->trueData = cipher;
            virtual_branch->is_virtual = true;
            virtual_branch->child[0] = NULL;
            virtual_branch->ID = temp_ID;
            virtual_branch->textID = temp_ID;
            virtual_branch->text = Com;
            text_position_map.insert(pair<int, string>(temp_ID,Com));
            temp_ID++;
            this->mBranch.push_back(virtual_branch);
        }
    }
    virtual double CalcuTextRelevancy(vector<double> weight1, vector<double> weight2);
    virtual double CalcuSpaceIncrease(Rectangle pre, Rectangle newr);
    virtual double CalcuTestSPaceRele(Branch *n1, Branch *n2);
    //virtual bool insertBranch(Branch *nBranch, Node *mNode, Node **newNode, int level);
    //virtual bool InsertLeaf(Branch *nBranch, Node **mRoot, int level);
    //virtual bool addBranch(Branch *nbranch, Node *MNode, Node **newNode);
    //virtual void initNode(Node *mNode);
    //virtual Node* allocNode();
    //virtual void splitNode(Node *mnode, Branch *nBranch, Node **newNode);
    //virtual void loadNodes(Node *nodeA, Node *nodeB, Part *parvars);
    //virtual void getBranchs(Node *mnode, Branch *nBranch, Part *parVars);
    //virtual void choosePart(Part *parvars, int minsize);
    //virtual void InitParVars(Part *parvars, int maxsize, int minsize);
    virtual Rectangle CombineRect(Rectangle *rect1, Rectangle *rect2);
    virtual vector<double> CombineKeyWords(vector<double> weight1, vector<double> weight2);
    virtual void PickSeeds(Part *parvars);
    virtual void Classify(int nIndex, int group, Part *parvars);
    virtual int pickInsertBranch(Branch *nBranch, Node * mNode);
    virtual Rectangle nodeCover(Node *mNode);
    //virtual void recoverNode();
    //virtual void searchFatherBranch(Branch *mBranch);
    //virtual void insertOtherLocation(int* data, int level, map<int, Branch*> leaf_position_map);
    //virtual void dataInsert(Branch *mBranch, int *data, int level);
    //virtual void recoverPath(Branch *mBranch);
    //virtual void searchLevelBranch(Branch *nBranch, int level, int *data);
    virtual bool overLap(Rectangle *rect1, Rectangle *rect2);
    virtual vector<double> CalcuKeyWeight(string &text);
    virtual void Delete();
    virtual void Lower(string &text);
    //virtual vector<Branch*> Merge_sort(vector<Branch*> branchs, int le);
};

#endif