#include "Node.h"
#include <stdio.h>
#include <string>
#include <queue>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <cctype>
#include <cmath>

using namespace std;


double Node::CalcuTextRelevancy(vector<double> weight1, vector<double> weight2)
{
    double rele = 0;
    double sum1 = 0,sum2 = 0;
    for(int i = 0; i < weight1.size(); i++)
    {
        rele += weight1[i]*weight2[i];
        sum1 += weight1[i]*weight1[i];
        sum2 += weight2[i]*weight2[i];
    }

    double tempsum = sqrt(sum1)*sqrt(sum2);

    return rele/tempsum;
}
//
double Node::CalcuSpaceIncrease(Rectangle pre, Rectangle newr)
{
    Rectangle r;
    for(int index = 0; index < 2; index++)
    {
        r.max_Rec[index] = max(pre.max_Rec[index], newr.max_Rec[index]);
        r.min_Rec[index] = min(pre.min_Rec[index], newr.min_Rec[index]);
    }
    double space_rele = (r.Area() - pre.Area()) / (pre.Area());

    return space_rele;
}

//
double Node::CalcuTestSPaceRele(Branch *n1, Branch *n2)
{
    double text = CalcuTextRelevancy(n1->weight,n2->weight);

    double space = CalcuSpaceIncrease(n1->m_rect, n2->m_rect);

    double rele = alpha * (1/(space+1)) + (1-alpha) * text;

    return rele;
}

//    
/*
bool Node::insertBranch(Branch *nBranch, Node *mNode, Node **newNode, int level)
{
    if(mNode->level > level)
    {+

        Node *otherNode;

        int index = pickInsertBranch(nBranch, mNode);
        bool childWasSplit = insertBranch(nBranch,, &otherNode,level);

        if(!childWasSplit)
        {
            mNode->mBranch[index]->rectUpdate(&(nBranch->m_rect));
            mNode->mBranch[index]->keyWeightUpdate(nBranch);
            mNode->mBranch[index]->child->invertedFileInit();

            return false;
        }
        else//孩子节点需要请增加一个分支
        {
            mNode->mBranch[index]->m_rect = nodeCover(mNode);
            Branch *branch = new Branch();
            otherNode->parent = branch;
            branch->child = otherNode;
            branch->m_rect = nodeCover(otherNode);

            return(addBranch(branch,mNode,newNode));
        }
    }
    else if(mNode->level == level)
    {
        return addBranch(nBranch,mNode,newNode);
    }
    else
    {
        return false;
    }
}
//

bool Node::InsertLeaf(Branch *nBranch, Node **mRoot, int level)
{
    Node *newNode;
    if(insertBranch(nBranch,*mRoot,&newNode, level))
    {
        Node *newRoot = allocNode();
        newRoot->level = (*mRoot)->level + 1;
        Branch *newBranch = new Branch();

        newBranch->m_rect = nodeCover(*mRoot);
        newBranch->child = *mRoot;
        (*mRoot)->parent = newBranch;
        addBranch(newBranch, newRoot, NULL);

        Branch *newBranch2 = new Branch();
        newBranch2->m_rect = nodeCover(newNode);
        newBranch2->child = newNode;
        newNode->parent = newBranch2;
        addBranch(newBranch2,newRoot,NULL);

        *mRoot = newRoot;
        return true;
    }
    return false;
}
//

bool Node::addBranch(Branch *nbranch, Node *MNode, Node **newNode)
{
    if(MNode->mBranch.size() < MAX_SIZE)
    {
        nbranch->mNode = MNode;
        MNode->mBranch[MNode->count] = nbranch;
        MNode->setCount();
        return false;
    }
    else//当前节点也需要分裂，向上递归
    {
        splitNode(MNode,nbranch,newNode);
        return true;
    }
}
//
void Node::initNode(Node *mNode)
{
    mNode->setCount();
    mNode->SetLevel();
}

//
Node* Node::allocNode()
{
    Node *newNode;
    newNode = new Node;
    initNode(newNode);
    return newNode;
}

void Node::splitNode(Node *mnode, Branch *nBranch, Node **newNode)
{
    Part localVars;
    Part *parvars = &localVars;
    getBranchs(mnode,nBranch,&localVars);

    choosePart(parvars,MIN_SIZE);

    *newNode = allocNode();
    (*newNode)->level = mnode->level;

    mnode->count = 0;
    loadNodes(mnode, *newNode,parvars);


}
void Node::loadNodes(Node *nodeA, Node *nodeB, Part *parvars)
{
    for(int index = 0; index < parvars->m_total; index++)
    {
        int targetNodeIndex = parvars->m_part[index];
        Node *targetNodeArray[] = {nodeA, nodeB};

        bool nodeWadSplit = addBranch(parvars->mBranchs[index], targetNodeArray[targetNodeIndex],NULL);

    }
}

//
void Node::getBranchs(Node *mnode, Branch *nBranch, Part *parVars)
{
    for(int index = 0; index < MAX_SIZE; index++)
    {
        parVars->mBranchs[index] = mnode->mBranch[index];
    }
    parVars->mBranchs[MAX_SIZE] = nBranch;
    parVars->m_branchCount = MAX_SIZE + 1;

    parVars->m_coverSplit = parVars->mBranchs[0]->m_rect;
    for(int index = 1; index < MAX_SIZE + 1; index++)
    {
        parVars->m_coverSplit = CombineRect(&parVars->m_coverSplit, &parVars->mBranchs[index]->m_rect);
    }
    parVars->m_coverSplitArea = parVars->m_coverSplit.Area();
}

//
void Node::choosePart(Part *parvars, int minsize)
{
    int group, chosen = 0, bettergroup = 0;
    InitParVars(parvars, parvars->m_branchCount, minsize);
    PickSeeds(parvars);
    double bestDiff = -1;
    while(((parvars->m_count[0] + parvars->m_count[1]) < parvars->m_total)
        && (parvars->m_count[0] < (parvars->m_total - parvars->min_Size))
        && (parvars->m_count[1] < (parvars->m_total - parvars->min_Size)))
    {
        bestDiff = -1;
        for(int index = 0; index < parvars->m_total; index++)
        {
            if(Part::NOT_TAKEN == parvars->m_part[index])
            {
                Rectangle *curRect = &(parvars->mBranchs[index]->m_rect);
                Rectangle rect0 = CombineRect(curRect,&(parvars->m_cover[0]));
                Rectangle rect1 = CombineRect(curRect,&(parvars->m_cover[1]));
                double increase0 = rect0.Area() - parvars->m_area[0];
                double increase1 = rect1.Area() - parvars->m_area[1];
                double diff = increase1 - increase0;
                if(diff >= 0)
                {
                    group = 0;
                }
                else
                {
                    group = 1;
                    diff = -diff;
                }

                if(diff > bestDiff)
                {
                    bestDiff = diff;
                    chosen = index;
                    bettergroup = group;
                }
                else if((diff == bestDiff) && (parvars->m_count[group] < parvars->m_count[bettergroup]))
                {
                    chosen = index;
                    bettergroup = group;
                }

            }
        }

        Classify(chosen,bettergroup,parvars);
    }

    if(parvars->m_count[0] + parvars->m_count[1] < parvars->m_total)
    {
        if(parvars->m_count[0] >= parvars->m_total - parvars->min_Size)
        {
            group = 1;
        }
        else
        {
            group = 0;
        }
        for (int index = 0; index < parvars->m_total; index++)
        {
            if(Part::NOT_TAKEN == parvars->m_part[index])
            {
                Classify(index, group, parvars);
            }
        }
    }
}


//
void Node::InitParVars(Part *parvars, int maxsize, int minsize)
{
    parvars->m_count[0] = parvars->m_count[1] = 0;
    parvars->m_count[0] = parvars->m_count[1] = 0;
    parvars->m_area[0] = parvars->m_area[1] =
    parvars->m_total = maxsize;
    parvars->min_Size = minsize;
    for(int index=0; index < maxsize; ++index)
    {
        parvars->m_part[index] = Part::NOT_TAKEN;
    }
}
*/
//
Rectangle Node::CombineRect(Rectangle *rect1, Rectangle *rect2)
{
    Rectangle newRect;
    for(int index = 0; index < 2; index++)
    {
        newRect.min_Rec[index] = min(rect1->min_Rec[index], rect2->min_Rec[index]);
        newRect.max_Rec[index] = max(rect1->max_Rec[index], rect2->max_Rec[index]);
    }

    return newRect;
}
//  
vector<double> Node::CombineKeyWords(vector<double> weight1, vector<double> weight2)
{
    vector<double> newWeight;
    for(int index = 0; index < weight1.size(); index++)
    {
        newWeight[index] = max(weight1[index],weight2[index]);
    }

    return newWeight;
}

//
void Node::PickSeeds(Part *parvars)
{
    int seed0 = 0;
    int seed1 = 1;
    double worst,waste;
    int _size = MAX_SIZE + 1;
    vector<double> area(_size);

    for(int index = 0; index < parvars->m_total; index++)
    {
        area[index] = parvars->mBranchs[index]->m_rect.Area();
    }

    worst = - parvars->m_coverSplitArea - 1;

    for(int indexA = 0; indexA < parvars->m_total - 1; indexA++)
    {
        for(int indexB = indexA + 1; indexB < parvars->m_total; indexB++)
        {
            Rectangle oneRect = CombineRect(&parvars->mBranchs[indexA]->m_rect, &parvars->mBranchs[indexB]->m_rect);
            waste = oneRect.Area() - area[indexA] - area[indexB];
            double textRele = CalcuTextRelevancy(parvars->mBranchs[indexA]->weight,parvars->mBranchs[indexB]->weight);
            waste += (1/textRele);

            if(waste > worst)
            {
                worst = waste;
                seed0 = indexA;
                seed1 = indexB;
            }
        }
    }

    Classify(seed0, 0, parvars);
    Classify(seed1, 1, parvars);
}
//
void Node::Classify(int nIndex, int group, Part *parvars)
{
    parvars->m_part[nIndex] = group;
    if(parvars->m_count[group] == 0)
    {
        parvars->m_cover[group] = parvars->mBranchs[nIndex]->m_rect;
    }
    else
    {
        parvars->m_cover[group] = CombineRect(&parvars->mBranchs[nIndex]->m_rect, &parvars->m_cover[group]);
    }
    if(group == 0)
    {
        parvars->weight0 = CombineKeyWords(parvars->weight0, parvars->mBranchs[nIndex]->weight);
    }
    else if(group == 1)
    {
        parvars->weight1 = CombineKeyWords(parvars->weight1,parvars->mBranchs[nIndex]->weight);
    }

    parvars->m_area[group] = parvars->m_cover[group].Area();

    parvars->m_count[group]++;
}

//
int Node::pickInsertBranch(Branch *nBranch, Node * mNode)
{
    int best = 0;
    double bestRele = 0;
    for(int index = 0; index < mNode->mBranch.size(); index++)
    {
        double tempRele = CalcuTestSPaceRele(nBranch,mNode->mBranch[index]);
        if (tempRele > bestRele)
        {
            best = index;
        }
    }
    return best;
}

Rectangle Node::nodeCover(Node *mNode)
{
    Rectangle r = mNode->mBranch[0]->m_rect;
    for(int index = 1; index < mNode->mBranch.size(); index++)
    {
        mNode->rectUpdate(mNode->mBranch[index]);
        r = mNode->m_rect;
    }
    return r;
}

//
//
/*
void Node::searchLevelBranch(Branch *nBranch, int level, int *data)
{
    Node *tempNode = nBranch->mNode;
    while(tempNode != NULL)
    {
        if(tempNode->level == level)
        {
            for(int index = 0; index < tempNode->mBranch.size(); index++)
            {
                Branch* temp_Branch = tempNode->mBranch[index];
                bool judge = temp_Branch->is_virtual;
                if(judge)
                {
                    tempNode->mBranch[index]->trueData = *data;
                    break;
                }
            }
        }
        else
        {
            tempNode = tempNode->parent->mNode;
        }

    }
}

*/
//
bool Node::overLap(Rectangle *rect1, Rectangle *rect2)
{
    for(int index = 0; index < 2; index++)
    {
        if((rect1->min_Rec[index] > rect2->max_Rec[index]) || (rect2->min_Rec[index] > rect1->max_Rec[index]))
        {
            return false;
        }


    }
    return true;
}
//

void Node::Lower(string& text)
{
    for (char& c : text)
    {
        c = tolower(c);
    }
}
//
vector<double> Node::CalcuKeyWeight(string &text)
{
    int dic_num = int(dic_str.size());
    vector<double> weight(dic_num,0);
    istringstream input(text);
    string temp;
    vector<string> tempstr;
    while(input >> temp)
        tempstr.push_back(temp);

    int i = 0, maxnum = 0;
    for(string str1: dic_str)
    {
        Lower(str1);
        for(string str2 : tempstr)
        {
            Lower(str2);
            if(str1 == str2)
            {
                weight[i] ++;
            }
        }
        if(weight[i] > maxnum)
        {
            maxnum = int(weight[i]);
        }
        i++;
    }

    for(int i = 0; i <weight.size(); i++)
    {
        weight[i] /= maxnum;
    }

    return weight;
}

//
/*
vector<Branch*> Node::Merge_sort(Branch **branchs, int le)
{

    Branch *temp_branchs1 = (Branch*)malloc(le * sizeof(Branch));
    Branch *temp_branchs2 = (Branch*)malloc(le * sizeof(Branch));
    for(int index = 0; index < le; index++)
    for(int index = 0; index < le; index++)
    {
        temp_branchs1[index] = branchs[index];
    }
    Branch *privious = temp_branchs1;
    int seg = 0,start = 0;
    for(seg = 1; seg < le; seg += seg)
    {
        for(start = 0; start < le; start += seg+seg)
        {
            int low = start, mid = min(start+seg, le),high = min(start+seg+seg,le);
            int k = low;
            int start1 = low, end1 = mid;
            int start2 = mid, end2 = high;
            while (start1 < end1 &&start2 < end2)
            {
                temp_branchs2[k++] = temp_branchs1[start1] < temp_branchs1[start2] ? temp_branchs1[start1++] : temp_branchs1[start2++];
            }
            while(start1 < end1)
            {
                temp_branchs2[k++] = temp_branchs1[start1++];
            }
            while( start2 < end2)
            {
                temp_branchs2[k++] = temp_branchs1[start2++];
            }
        }
        Branch *temp = temp_branchs1;
        temp_branchs1 = temp_branchs2;
        temp_branchs2 = temp;

    }
    if(temp_branchs1 != privious)
    {
        int i = 0;
        for( i = 0; i < le; i++)
        {
            temp_branchs2[i] = temp_branchs1[i];
        }
        temp_branchs2 = temp_branchs1;
    }
    for(int i = 0; i < le; i++)
    {
        branchs[i] = temp_branchs1[i];
    }
    free(temp_branchs2);

    return branchs;
}
*/
//
void Node::Delete()
{

}


//weight取最大的更新函数
//space取最大的更新函数还没写



