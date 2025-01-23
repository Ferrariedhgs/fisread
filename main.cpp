#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <unistd.h>
#include <map>
#include <string>
#include <fstream>


using namespace std;


double triangle(double a, double b, double c, double in);
double trapezoid(double a, double b, double c, double d, double in);
double triangle_center(double a, double b, double c, double in);
double trapezoid_center(double a, double b, double c, double d, double in);

ifstream fis("fuzzy.fis");//matlab .fis file
ifstream data("input.txt");//number of pairs and the inputs on each line n,e,de,e,de,...
ofstream o("output.txt");

int main()
{
    //fis init
    if(!fis.is_open()) {cout<<"failed to open .fis file"; return 1;}
    if(!data.is_open()) {cout<<"failed to open input.txt file"; return 1;}
    if(!o.is_open()) {cout<<"failed to open output.txt file"; return 1;}

    string currentline;
    getline(fis,currentline);//skip [System] header

    getline(fis,currentline);//get name
    size_t search_pos = currentline.find("Name=");
    if(search_pos != string::npos)
        currentline.erase(search_pos, 5);
    cout<<currentline<<" loaded"<<endl;

    getline(fis,currentline);//get inference type
    currentline.erase(search_pos, 5);
    if(currentline=="'mamdani'")
        cout<<"Mamdani inference"<<endl;
    else
    {
        cout<<currentline<<" not supported";
        return 1;
    }

    getline(fis,currentline);//get version
    currentline.erase(search_pos, 8);
    if(currentline=="2.0")
        cout<<"version 2.0"<<endl;
    else
    {
        cout<<"version "<<currentline<<" not supported";
        return 1;
    }

    getline(fis,currentline);//get nr of inputs
    currentline.erase(search_pos, 10);
    if(currentline=="2")
        cout<<"2 inputs"<<endl;
    else
    {
        cout<<"only 2 inputs supported";
        return 1;
    }

    getline(fis,currentline);//get nr of inputs
    currentline.erase(search_pos, 11);
    if(currentline=="1")
        cout<<"1 output"<<endl;
    else
    {
        cout<<"only 1 output supported";
        return 1;
    }

    getline(fis,currentline);//get nr of rules
    currentline.erase(0, 9);
    int rules_nr=stoi(currentline);
    cout<<rules_nr<<" rules"<<endl;

    for(int i=0;i<5;i++)//skip and,or,imp,agg,defuzz
        getline(fis,currentline);

    //input 1
    getline(fis,currentline);//skip blank row
    getline(fis,currentline);//skip input header

    cout<<endl;
    getline(fis,currentline);//get name
    currentline.erase(0, 6);
    string input1_name=currentline;
    input1_name.pop_back();
    cout<<input1_name<<endl;

    getline(fis,currentline);//get range
    currentline.erase(search_pos, 7);
    double input1_range[2];
    input1_range[0]=stod(currentline);
    string aux_delete=strchr(currentline.c_str(), ' ');
    currentline.erase(0, 1);
    input1_range[1]=stod(currentline);
    cout<<"["<<input1_range[0]<<","<<input1_range[1]<<"] range"<<endl;

    getline(fis,currentline);//get nr of mf
    currentline.erase(0, 7);
    int input1_mf_nr=stoi(currentline);
    cout<<input1_mf_nr<<" membership functions"<<endl;

    string input1_mf_type[input1_mf_nr];
    double input1_mf_values[input1_mf_nr][4];
    for(int i=0;i<input1_mf_nr;i++)//get each mf
    {
        getline(fis,currentline);
        aux_delete=strchr(currentline.c_str(), ':');
        currentline.erase(0, aux_delete.length());
        currentline=aux_delete;
        aux_delete=currentline.substr(0, 9);
        if(aux_delete==":'trapmf'")
        {
            input1_mf_type[i]="trapmf";
            currentline.erase(0,11);
            input1_mf_values[i][0]=stod(currentline);
            aux_delete=strchr(currentline.c_str(), ' ');
            currentline=aux_delete.erase(0, 1);
            input1_mf_values[i][1]=stod(currentline);
            aux_delete=strchr(currentline.c_str(), ' ');
            currentline=aux_delete.erase(0, 1);
            input1_mf_values[i][2]=stod(currentline);
            aux_delete=strchr(currentline.c_str(), ' ');
            currentline=aux_delete.erase(0, 1);
            input1_mf_values[i][3]=stod(currentline);

            cout<<"trapmf("<<input1_mf_values[i][0]<<","<<input1_mf_values[i][1]<<","<<input1_mf_values[i][2]<<","<<input1_mf_values[i][3]<<")"<<endl;
        }
        else if(aux_delete==":'trimf',")
        {
            input1_mf_type[i]="trimf";
            currentline.erase(0,10);
            input1_mf_values[i][0]=stod(currentline);
            aux_delete=strchr(currentline.c_str(), ' ');
            currentline=aux_delete.erase(0, 1);
            input1_mf_values[i][1]=stod(currentline);
            aux_delete=strchr(currentline.c_str(), ' ');
            currentline=aux_delete.erase(0, 1);
            input1_mf_values[i][2]=stod(currentline);

            cout<<"trimf("<<input1_mf_values[i][0]<<","<<input1_mf_values[i][1]<<","<<input1_mf_values[i][2]<<")"<<endl;
        } else return 1;

    }


    //input 2
    getline(fis,currentline);//skip blank row
    getline(fis,currentline);//skip input header

    cout<<endl;
    getline(fis,currentline);//get name
    currentline.erase(0, 6);
    string input2_name=currentline;
    input2_name.pop_back();
    cout<<input2_name<<endl;

    getline(fis,currentline);//get range
    currentline.erase(search_pos, 7);
    double input2_range[2];
    input2_range[0]=stod(currentline);
    aux_delete=strchr(currentline.c_str(), ' ');
    currentline.erase(0, 1);
    input2_range[1]=stod(currentline);
    cout<<"["<<input2_range[0]<<","<<input2_range[1]<<"] range"<<endl;

    getline(fis,currentline);//get nr of mf
    currentline.erase(0, 7);
    int input2_mf_nr=stoi(currentline);
    cout<<input2_mf_nr<<" membership functions"<<endl;

    string input2_mf_type[input1_mf_nr];
    double input2_mf_values[input1_mf_nr][4];
    for(int i=0;i<input2_mf_nr;i++)//get each mf
    {
        getline(fis,currentline);
        aux_delete=strchr(currentline.c_str(), ':');
        currentline.erase(0, aux_delete.length());
        currentline=aux_delete;
        aux_delete=currentline.substr(0, 9);
        if(aux_delete==":'trapmf'")
        {
            input2_mf_type[i]="trapmf";
            currentline.erase(0,11);
            input2_mf_values[i][0]=stod(currentline);
            aux_delete=strchr(currentline.c_str(), ' ');
            currentline=aux_delete.erase(0, 1);
            input2_mf_values[i][1]=stod(currentline);
            aux_delete=strchr(currentline.c_str(), ' ');
            currentline=aux_delete.erase(0, 1);
            input2_mf_values[i][2]=stod(currentline);
            aux_delete=strchr(currentline.c_str(), ' ');
            currentline=aux_delete.erase(0, 1);
            input2_mf_values[i][3]=stod(currentline);

            cout<<"trapmf("<<input2_mf_values[i][0]<<","<<input2_mf_values[i][1]<<","<<input2_mf_values[i][2]<<","<<input2_mf_values[i][3]<<")"<<endl;
        }
        else if(aux_delete==":'trimf',")
        {
            input2_mf_type[i]="trimf";
            currentline.erase(0,10);
            input2_mf_values[i][0]=stod(currentline);
            aux_delete=strchr(currentline.c_str(), ' ');
            currentline=aux_delete.erase(0, 1);
            input2_mf_values[i][1]=stod(currentline);
            aux_delete=strchr(currentline.c_str(), ' ');
            currentline=aux_delete.erase(0, 1);
            input2_mf_values[i][2]=stod(currentline);

            cout<<"trimf("<<input2_mf_values[i][0]<<","<<input2_mf_values[i][1]<<","<<input2_mf_values[i][2]<<")"<<endl;
        } else return 1;

    }

    getline(fis,currentline);//skip blank row
    getline(fis,currentline);//skip input header

    cout<<endl;
    getline(fis,currentline);//get name
    currentline.erase(0, 6);
    string output_name=currentline;
    output_name.pop_back();
    cout<<output_name<<endl;

    getline(fis,currentline);//get range
    currentline.erase(search_pos, 7);
    double output_range[2];
    output_range[0]=stod(currentline);
    aux_delete=strchr(currentline.c_str(), ' ');
    currentline.erase(0, 1);
    output_range[1]=stod(currentline);
    cout<<"["<<output_range[0]<<","<<output_range[1]<<"] range"<<endl;

    getline(fis,currentline);//get nr of mf
    currentline.erase(0, 7);
    int output_mf_nr=stoi(currentline);
    cout<<output_mf_nr<<" membership functions"<<endl;

    string output_mf_type[output_mf_nr];
    double output_mf_values[output_mf_nr][4];
    for(int i=0;i<output_mf_nr;i++)//get each mf
    {
        getline(fis,currentline);
        aux_delete=strchr(currentline.c_str(), ':');
        currentline.erase(0, aux_delete.length());
        currentline=aux_delete;
        aux_delete=currentline.substr(0, 9);
        if(aux_delete==":'trapmf'")
        {
            output_mf_type[i]="trapmf";
            currentline.erase(0,11);
            output_mf_values[i][0]=stod(currentline);
            aux_delete=strchr(currentline.c_str(), ' ');
            currentline=aux_delete.erase(0, 1);
            output_mf_values[i][1]=stod(currentline);
            aux_delete=strchr(currentline.c_str(), ' ');
            currentline=aux_delete.erase(0, 1);
            output_mf_values[i][2]=stod(currentline);
            aux_delete=strchr(currentline.c_str(), ' ');
            currentline=aux_delete.erase(0, 1);
            output_mf_values[i][3]=stod(currentline);

            cout<<"trapmf("<<output_mf_values[i][0]<<","<<output_mf_values[i][1]<<","<<output_mf_values[i][2]<<","<<output_mf_values[i][3]<<")"<<endl;
        }
        else if(aux_delete==":'trimf',")
        {
            output_mf_type[i]="trimf";
            currentline.erase(0,10);
            output_mf_values[i][0]=stod(currentline);
            aux_delete=strchr(currentline.c_str(), ' ');
            currentline=aux_delete.erase(0, 1);
            output_mf_values[i][1]=stod(currentline);
            aux_delete=strchr(currentline.c_str(), ' ');
            currentline=aux_delete.erase(0, 1);
            output_mf_values[i][2]=stod(currentline);

            cout<<"trimf("<<output_mf_values[i][0]<<","<<output_mf_values[i][1]<<","<<output_mf_values[i][2]<<")"<<endl;
        } else return 1;

    }


    //rules
    getline(fis,currentline);//skip blank line
    getline(fis,currentline);//skip rules header
    int rules[input1_mf_nr][input2_mf_nr];
    for(int i=input1_mf_nr-1;i>=0;i--)
        for(int j=input2_mf_nr-1;j>=0;j--)
        {
            getline(fis,currentline);
            currentline.erase(0,5);
            rules[i][j]=stod(currentline);
        }

    //fis init end


    o<<input1_name<<"\t\t"<<input2_name<<"\t\t"<<output_name<<endl;
    int n;
    data>>n;//number of pairs
    cout<<"\n\n\n"<<n<<" inputs"<<endl;
    for(int y=0;y<n;y++)//calculate for every pair
    {
    double in1=0;
    double in2=0;
    data>>in1;
    data>>in2;
    double fuzz_in1[input1_mf_nr];//calculate activation of every function for input1
    for(int i=0;i<input1_mf_nr;i++)
    {
        if(input1_mf_type[i]=="trimf")
            fuzz_in1[i]=triangle(input1_mf_values[i][0],input1_mf_values[i][1],input1_mf_values[i][2],in1);
        else if(input1_mf_type[i]=="trapmf")
            fuzz_in1[i]=trapezoid(input1_mf_values[i][0],input1_mf_values[i][1],input1_mf_values[i][2],input1_mf_values[i][3],in1);
            //cout<<"in1 "<<fuzz_in1[i]<<endl;
    }

    double fuzz_in2[input2_mf_nr];//calculate activation of every function for input2
    for(int i=0;i<input2_mf_nr;i++)
    {
        if(input2_mf_type[i]=="trimf")
            fuzz_in2[i]=triangle(input2_mf_values[i][0],input2_mf_values[i][1],input2_mf_values[i][2],in2);
        else if(input1_mf_type[i]=="trapmf")
            fuzz_in2[i]=trapezoid(input2_mf_values[i][0],input2_mf_values[i][1],input2_mf_values[i][2],input2_mf_values[i][3],in2);
        //cout<<"in2 "<<fuzz_in2[i]<<endl;
    }

    double inference_matrix[input1_mf_nr][input2_mf_nr];
    for (int i = 0; i < input1_mf_nr; i++)
    {
        for (int j = 0; j < input2_mf_nr; j++)
        {
            if (fuzz_in1[i]>fuzz_in2[j]) inference_matrix[i][j] = fuzz_in2[j];
            else inference_matrix[i][j] = fuzz_in1[i];
        }

    }

    double u[output_mf_nr];
    for(int x=0;x<output_mf_nr;x++)
    {
        u[x]=0;
        for (int i = 0; i < input1_mf_nr; i++)
        {
            for (int j = 0; j < input2_mf_nr; j++)
            {
                if ((rules[i][j]==x)&&(u[x]<inference_matrix[i][j]))//output_mf_type[x]
                    u[x] = inference_matrix[i][j];
            }
        }

    }

    double centers[output_mf_nr];//calculate activation of every function for output
    for(int i=0;i<output_mf_nr;i++)
    {
        if(output_mf_type[i]=="trimf")
            centers[i]=triangle_center(output_mf_values[i][0],output_mf_values[i][1],output_mf_values[i][2],u[i]);
        else if(output_mf_type[i]=="trapmf")
            centers[i]=trapezoid_center(output_mf_values[i][0],output_mf_values[i][1],output_mf_values[i][2],output_mf_values[i][3],u[i]);
    }

    double num=0;
    double den=0;
    for(int i=0;i<output_mf_nr;i++)
    {
        num+=u[i]*centers[i];
        den+=u[i];
    }
    double deffuzzed=num/den;
    o<<in1<<"\t\t"<<in2<<"\t\t"<<deffuzzed<<endl;
    cout<<"pair number "<<y+1<<endl;
    }
    fis.close();
    data.close();
    o.close();
    return 0;
}

double triangle(double a, double b, double c, double in)
{
    if(in<a||in>c) return 0;
    else if(in>=a&&in<=b) return (in-a)/(b-a);
    else return (c-in)/(c-b);
}

double trapezoid(double a, double b, double c, double d, double in)
{
    if (in<a) return 0;
    else if ((in>= a) && (in<= b)) return (in - a) / (b - a);
    else if ((in>b) && (in<c)) return 1;
    else if ((in>= c) && (in<= d)) return (d - in) / (d - c);
    else return 0;
}

double triangle_center(double a, double b, double c, double in)
{
    double y1, y2;
    if (in != 0)
    {
    y1 = in * (b - a) + a;
    y2 = c - (in * (c - b));
    return (y1 + y2) / 2;
    }
    else return 0;
}

double trapezoid_center(double a, double b, double c, double d, double in)
{
    double y1, y2;
    if (in != 0)
    {
    y1 = in * (b - a) + a;
    y2 = d - in * (d - c);
    return (y1 + y2) / 2;
    }
    else return 0;
}

