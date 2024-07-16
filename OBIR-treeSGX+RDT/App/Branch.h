#ifndef BRANCH_H
#define BRANCH_H

#include "Pdefault.h"
#include <string>
#include <cctype>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;
typedef string Datatype;

struct Keyword {
    string name;
    double weight;

    Keyword() = default;

    Keyword(const string& name, double weight)
            : name(name), weight(weight)
    {
    }

    bool operator==(const Keyword& other) const
    {
        return false;
    }
};

struct Rectangle {
    double min_Rec[2];
    double max_Rec[2];
    double Area()
    {
        double area = 1;
        for(int index = 0; index < 2; index++)
        {
            area *= (this->max_Rec[index] - this->min_Rec[index]);
        }

        return area;
    }



    Rectangle() = default;

    bool operator==(const Rectangle& other) const
    {
        return false;
    }
};

struct PointData {
    int id;
    Rectangle rect;
    vector<Keyword> keywords;

    PointData() = default;



    bool operator==(const PointData& other) const
    {
        return id == other.id && rect == other.rect && keywords == other.keywords;
    }
};


//1、rect的更新函数，即在一个rect中插入另一个rect
//2、两个rect的面积差的计算
//3、两个rect是否有重合。
//三个函数的编写


struct InvertedFile {
    vector<Keyword> words;
    int index;


    InvertedFile() = default;

    InvertedFile(const vector<Keyword>& words, int index)
            : words(words), index(index)
    {
    }

    bool operator==(const InvertedFile& other) const
    {
        return words == other.words && index == other.index;
    }
};

class Node;

class Branch {

public:

    Node* childNode;
    Node* curNode;
    Rectangle m_rect;
    string text;
    vector<double> weight;
    string trueData;
    bool isEmpty;
    bool is_virtual;
    int ID;
    int level;
    Branch* pointBranch;
    vector<Branch*> child;
    Branch* partent;
    int textID;
    Branch()
    {
        textID = 0;
        isEmpty = true;
        for (int i = 0; i < 2; i++)
        {
            m_rect.max_Rec[i] = 0;
            m_rect.min_Rec[i] = 0;
        }
        for (int i = 0; i < 405; i++)
        {
            weight.push_back(0);
        }
        level = 0;
        is_virtual = false;
        ID = -1;
        partent = NULL;
        pointBranch = NULL;
        childNode = NULL;
        trueData = string("\0");
        text = string("\0");
        curNode = NULL;
    }

    //
    bool operator<(const Branch& other) const
    {
        double x1 = (m_rect.min_Rec[0] + m_rect.max_Rec[0]) / 2;
        double y1 = (m_rect.min_Rec[1] + m_rect.max_Rec[1]) / 2;
        double x2 = (other.m_rect.min_Rec[0] + other.m_rect.max_Rec[0]) / 2;
        double y2 = (other.m_rect.min_Rec[1] + other.m_rect.max_Rec[1]) / 2;
        if (x1 == x2)
        {
            return y1 < y2;
        }
        else
        {
            return x1 < x2;
        }
    }
    //
    void textUpdate(Branch* mbranch)
    {
        this->text = string("flag");
    }
    void LowerText(string &text)
    {
        for(char &c : text)
        {
            c = tolower(c);
        }
    }

    void setMyLevel()
    {
        Branch* tempBranch = this->child[0];
        int _level = 0;
        while (tempBranch != NULL)
        {
            _level++;
        }
        this->level = _level;
    }
    int levenshteinDistance(const string& s1, const string& s2) {
        int m = s1.length();
        int n = s2.length();

        // Create a matrix to store distances
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                int cost = (s1[i - 1] != s2[j - 1]);
                dp[i][j] = min({ dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + cost });
            }
        }

        return dp[m][n];
    }

    double similarity(const string& s1, const string& s2) {
        int distance = levenshteinDistance(s1, s2);
        return 1.0 - static_cast<double>(distance) / max(s1.length(), s2.length());
    }

    void CalcuKeyWordRele(string& text)
    {
        this->text = text.c_str();
        int _size = dic_str.size();
        for (int i = 0; i < _size; i++)
        {
            this->weight[i] = similarity(text, dic_str[i]);
            double rele = this->weight[i];
        }
    }

    void CalcuKeyWordWeight(string &text)
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
            LowerText(str1);
            for(string str2 : tempstr)
            {
                LowerText(str2);
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
        this->weight = weight;
    }


    void keyWeightUpdate(Branch *nBranch)
    {
        for(int index = 0; index < this->weight.size(); index++)
        {
            this->weight[index] = max(this->weight[index], nBranch->weight[index]);
        }
    }

    void rectUpdate(Rectangle *nRect)
    {
        for(int index = 0; index < 2; index++)
        {
            this->m_rect.min_Rec[index] = min(this->m_rect.min_Rec[index], nRect->min_Rec[index]);
            this->m_rect.max_Rec[index] = max(this->m_rect.max_Rec[index], nRect->max_Rec[index]);
        }
    }

//

    bool operator==(const Branch& other) const
    {
        return childNode == other.childNode && m_rect == other.m_rect && weight == other.weight && trueData == other.trueData && isEmpty == other.isEmpty && is_virtual == other.is_virtual && ID == other.ID && level == other.level && pointBranch == other.pointBranch && child == other.child && partent == other.partent;
    }
};

struct Part
{
    enum { NOT_TAKEN = -1};
    int m_part[MAX_SIZE + 1];
    int m_total;
    int min_Size;
    int m_count[2];
    Rectangle m_cover[2];
    double m_area[2];
    vector<double> weight0;
    vector<double> weight1;
    vector<Branch*> mBranchs;
    int m_branchCount;
    Rectangle m_coverSplit;
    double m_coverSplitArea;
};
struct kNNUtil {
    bool isleaf;
    double rele;
    Branch* pBranch;

};
#endif