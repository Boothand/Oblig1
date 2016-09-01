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
        
        int curveCount = 0;
        bool readingVerts = false;
        bool readingCurves = false;
        float x, y, z;
        vector<int> vertexAmountList;
        
        float unit = 0.001f;
        
        string str;        
        
        enum State { countCurves, countVertices, write, done };
        State stage = countCurves;
        
        if (reader.is_open())
        {
            while (stage != done)
            {
                switch (stage)
                {
                    case countCurves:
                    {
                        while (!reader.eof())
                        {
                            getline(reader, str);
                            
                            if (strncmp(str.c_str(), ".KURVE", 6) == 0)
                            {
                                curveCount++;
                            }
                            
                            if (strncmp(str.c_str(), ".SLUTT", 6) == 0)
                            {
                                reader.seekg(0, ios::beg);  //Read from beginning again.
                                stage = countVertices;
                                break;
                            }
                        }
                        break;
                    }
                    case countVertices:
                    {
                        while (!reader.eof())
                        {
                            getline(reader, str);
                            
                            if (strncmp(str.c_str(), ".KURVE", 6) == 0)
                            {
                                readingCurves = true;
                            }
                            
                            if (readingCurves)
                            {
                                if (strncmp(str.c_str(), "..NØ", 4) == 0)
                                {
                                    readingVerts = true;
                                    getline(reader, str);
                                }
                                
                                if (readingVerts)
                                {
                                    int vertsPerCurve = 0;
                                    while (true)
                                    {
                                        vertsPerCurve++;
                                        getline(reader, str);
                                        
                                        if (strncmp(str.c_str(), ".KURVE", 6) == 0)
                                        {
                                            vertexAmountList.push_back(vertsPerCurve);
                                            readingVerts = false;
                                            break;
                                        }
                                        
                                        if (strncmp(str.c_str(), ".SLUTT", 6) == 0)
                                        {
                                            vertexAmountList.push_back(vertsPerCurve);
                                            readingVerts = false;
                                            readingCurves = false;
                                            
                                            reader.seekg(0, ios::beg);  //Read from beginning again.
                                            
                                            stage = write;
                                            break;
                                        }
                                    }
                                    
                                    break;
                                }
                            }
                        }
                        break;
                    }  
                    case write:
                    {
                        int curveIndex = 0;
                        writer << curveCount << endl;
                        
                        while (!reader.eof())
                        {
                            getline(reader, str);
                            
                            if (strncmp(str.c_str(), ".KURVE", 6) == 0)
                            {
                                readingCurves = true;
                                readingVerts = false;
                                
                                curveIndex++;
                                writer << "Curve " << curveIndex << endl;
                                writer << vertexAmountList.front() << endl;
                                vertexAmountList.erase(vertexAmountList.begin());
                            }
                            
                            if (readingCurves && !readingVerts)
                            {
                                if (strncmp(str.c_str(), "..HØYDE", 7) == 0)
                                {
                                    string zStr = str.substr(8, (int)str.length() - 8);
                                    z = std::stof(zStr.c_str(), nullptr);
                                }
                                
                                if (strncmp(str.c_str(), "..NØ", 7) == 0)
                                {
                                    readingVerts = true;
                                    getline(reader, str);
                                }
                            }
                            
                            if (strncmp(str.c_str(), ".SLUTT", 6) == 0)
                            {
                                stage = done;
                                cout << "Converted " << curveCount << " curves." << endl;
                                break;
                            }
                            
                            if (readingVerts)
                            {
                                std:size_t splitPos = str.find(" ");
                                string yStr = str.substr(0, (int)splitPos);
                                string xStr = str.substr((int)splitPos + 1, (int)str.length() - (int)yStr.length());
                                
                                y = std::stof(yStr.c_str(), nullptr);
                                x = std::stof(xStr.c_str(), nullptr);
                                
                                x *= unit;
                                y *= unit;
                                z *= unit;                
                                
                                writer << "(" << x << ", " << y << ", " << z << ") (0.5, 0.5, 0.5) (1, 0)" << endl;
                            }
                        }
                        
                        break;
                    }
                    case done:
                    {
                        break;
                    }
                }
            }
        }
        
        writer.close();
}

int main(int argc, char *argv[])
{
    ConvertToVerticesTest("../Oblig1/SOSI/in.sos");
    
    ConvertToCurves("../Oblig1/SOSI/in.sos");
    
    return 0;
}
