#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cmath>
#include <vector>

using namespace std;

void ConvertToVerticesTest(int &minX, int &maxX, int &minY, int &maxY, string in = "../Oblig1/SOSI/in.sos", string out = "../Oblig1/SOSI/vertices.txt")
{    
    ifstream reader;
    ofstream writer;
    
    reader.open(in);
    writer.open(out);
    
    string str;
    
    int totalVertices = 0;
    float unit = 0.01f;
    int pass = 1;
    float z, y, x;
    
    if (reader.is_open() && pass < 3)
    {
        while (!reader.eof())
        {
            
            getline(reader, str);
            
            if (pass == 1 && strncmp(str.c_str(), "...MIN-NØ", 9) == 0)
            {
                std::size_t splitPos1 = str.find_first_of(" ");
                std::size_t splitPos2 = str.find_last_of(" ");
                
                string yStr = str.substr((int)splitPos1 + 1, str.length() - (int)splitPos2 + 1);
                string xStr = str.substr((int)splitPos2 + 1, (int)str.length() - (int)yStr.length() - 1);
                
                cout << "Min X: " << xStr << endl;
                cout << "Min Y: " << yStr << endl;
            }
            
            if (pass == 1 && strncmp(str.c_str(), "...MAX-NØ", 9) == 0)
            {
                std::size_t splitPos1 = str.find_first_of(" ");
                std::size_t splitPos2 = str.find_last_of(" ");
                
                string yStr = str.substr((int)splitPos1 + 1, str.length() - (int)splitPos2 + 1);
                string xStr = str.substr((int)splitPos2 + 1, (int)str.length() - (int)yStr.length() - 1);
                
                cout << "Max X: " << xStr << endl;
                cout << "Max Y: " << yStr << endl << endl;
                
                cout << "Set new Min X: ";
                cin >> minX;
                
                cout << "Set new Max X: ";
                cin >> maxX;
                
                cout << "Set new Min Y: ";
                cin >> minY;
                
                cout << "Set new Max Y: ";
                cin >> maxY;
            }
            
            if (pass == 2 && strncmp(str.c_str(), "..HØYDE", 7) == 0)
            {
                str = str.substr(8, (int)str.length() - 8);
                z = std::stof(str.c_str(), nullptr);
            }
            else if (strncmp(str.c_str(), "..NØ", 4) == 0)
            {
                getline(reader, str);
                std::size_t splitPos = str.find(" ");
                string yStr = str.substr(0, (int)splitPos);
                string xStr = str.substr((int)splitPos + 1, (int)str.length() - (int)yStr.length());
                
                y = std::stof(yStr.c_str(), nullptr);
                x = std::stof(xStr.c_str(), nullptr);
                
                x = x * unit;
                y = y * unit;
                z = z * unit;
                
                if (x >= minX &&
                    x <= maxX &&
                    y >= minY &&
                    y <= maxY)
                {
                    if (pass == 2)
                    {
                        x = round((double) x * 1000) / 1000;
                        y = round((double) y * 1000) / 1000;
                        z = round((double) z * 1000) / 1000;
                        writer << fixed;
                        writer << "(" << x << ", " << y << ", " << z << ") (0.5, 0.5, 0.5) (1, 0)" << endl;
                    }
                    else if (pass == 1)
                    {
                        totalVertices++;                        
                    }
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
    int minX = 10000;
    int minY = 6800000;
    int maxX = 40000;
    int maxY = 6900000;
    ConvertToVerticesTest(minX, maxX, minY, maxY, "../Oblig1/SOSI/in.sos");
    
//    ConvertToCurves("../Oblig1/SOSI/in.sos");
    
    return 0;
}
