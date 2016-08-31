#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cmath>
#include <vector>

using namespace std;

void ConvertToVerticesTest(string in = "../Oblig1/SOSI/in.sos", string out = "../Oblig1/SOSI/vertices.txt")
{    
    ifstream reader;
    ofstream writer;
    
    reader.open(in);
    writer.open(out);
    
    string str;
    
    int totalVertices = 0;
    float unit = 0.001f;
    int pass = 1;
    float z, y, x;
    
    if (reader.is_open() && pass < 3)
    {
        while (!reader.eof())
        {
            
            getline(reader, str);
            if (pass == 2 && strncmp(str.c_str(), "..HØYDE", 7) == 0)
            {
                str = str.substr(8, (int)str.length() - 8);
                z = std::stof(str.c_str(), nullptr);
//                cout << z << endl;
            }
            else if (strncmp(str.c_str(), "..NØ", 4) == 0)
            {
                if (pass == 2)
                {
                    getline(reader, str);
                    std:size_t splitPos = str.find(" ");
                    string yStr = str.substr(0, (int)splitPos);
                    string xStr = str.substr((int)splitPos + 1, (int)str.length() - (int)yStr.length());
                    
                    y = std::stof(yStr.c_str(), nullptr);
                    x = std::stof(xStr.c_str(), nullptr);
                    
    //                cout << y << endl;
    //                cout << x << endl;
                    
                    x = x * unit;
                    y = y * unit;
                    z = z * unit;
                    
                    writer << "(" << x << ", " << y << ", " << z << ") (0.5, 0.5, 0.5) (1, 0)" << endl;
                }
                
                if (pass == 1)
                {
                    totalVertices++;
                }
            }
            else if (strncmp(str.c_str(), ".KURVE", 6) == 0)
            {
                if (pass == 1)
                {
                    writer << totalVertices << endl;
                    reader.seekg(0, ios::beg);
                }
                
                pass++;
                continue;
            }
        }
        writer.close();
    }
    
    
    
    cout << "Total vertices converted: " << totalVertices << endl;
}

void ConvertToCurves(string in = "../Oblig1/SOSI/in.sos", string out = "../Oblig1/SOSI/curves.txt")
{
    ifstream reader;
    ofstream writer;
    
    reader.open(in);
    writer.open(out);
    
    float unit = 0.001f;
    int curveIndex = 0;
    int vertexIndex = 0;
    int pass = 1;
    bool readingCurves = false;
    bool readingVerts = false;
    bool foundStartCurve = false;
    
    int startCurveLineNum = 0;
    
    vector<int> vertexAmountArray;
    
    string str;
    float x, y, z;
    
    enum State { countCurves, countVertices, write, done };
    State stage = countCurves;
    
    if (reader.is_open())
    {
        while (!reader.eof() && stage != done)
        {
            getline(reader, str);
            
            
            switch (stage)
            {
            case countCurves:
                
                break;
            case countVertices:
                
                break;
                
            case write:
                
                break;
                
            case done:
                
                break;
            }
        }
    }
    
    return;
    
    
    
    
    if (reader.is_open())
    {
        while (!reader.eof() && pass < 4)
        {
            getline(reader, str);
            
            if (!foundStartCurve)
            {
                startCurveLineNum++;
            }
            
            if (strncmp(str.c_str(), ".SLUTT", 6) == 0)
            {
                if (pass == 3)
                {
                    writer << curveIndex << endl;
                    
                }
                reader.seekg(startCurveLineNum - 1, ios::beg);
                pass++;
                vertexIndex = 0;
                curveIndex = 0;
                readingCurves = false;
                readingVerts = false;
                
                continue;
            }
            
            if (strncmp(str.c_str(), ".KURVE", 6) == 0)
            {
                if (!foundStartCurve)
                {
                    foundStartCurve = true;
                }
                
                curveIndex++;
                if (pass > 1)
                {
                    writer << "Curve " << curveIndex << endl;
                    writer << vertexIndex << endl;
                }
                
                vertexIndex = 0;
                readingVerts = false;
                readingCurves = true;
            }
            
            if (readingCurves)
            {
                if (strncmp(str.c_str(), "..HØYDE", 7) == 0)
                {
                    str = str.substr(8, (int)str.length() - 8);
                    z = std::stof(str.c_str(), nullptr);
                }
                else if (strncmp(str.c_str(), "..NØ", 4) == 0)
                {
                    readingVerts = true;
                    getline(reader, str);
                }
            
                if (readingVerts)
                {
                    if (pass > 1)
                    {
                        std:size_t splitPos = str.find(" ");
                        string yStr = str.substr(0, (int)splitPos);
                        string xStr = str.substr((int)splitPos + 1, (int)str.length() - (int)yStr.length());
                        
                        y = std::stof(yStr.c_str(), nullptr);
                        x = std::stof(xStr.c_str(), nullptr);
                        
                        x = x * unit;
                        y = y * unit;
                        z = z * unit;                
                        
                        writer << "(" << x << ", " << y << ", " << z << ") (0.5, 0.5, 0.5) (1, 0)" << endl;
                    }
                    
                    vertexIndex++;
                }
            }
        }
        
        writer.close();
        cout << "Converted " << curveIndex << " curves." << endl;
    }
}

int main(int argc, char *argv[])
{
    //ConvertToVerticesTest("../Oblig1/SOSI/in.sos");
    
    ConvertToCurves("../Oblig1/SOSI/in.sos");
    
    return 0;
}
