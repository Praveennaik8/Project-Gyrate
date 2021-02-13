/***************************************************************************************************
Title      : PROJECT GYRATE
Authors    : 1.Praveen Naik
             2.Prasad Patil
             3.Prajwal Gouli
             4.Prashant Roy
Start Date : 15 / 11 / 2019
End Date   : 25 / 11 / 2019

University : KLE Technological University , Hubli
Guidance   : Asst. Prof. Prakash Hegade

Purpose    : To simulate the Cyclone attack scenario

Copyright (c) 2019
Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sub-license, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
***********************************************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<malloc.h>
#include<string.h>
#include<math.h>
#include<limits.h>
#include<stdbool.h>

#define SUCCESS 1
#define FAILURE -1
#define OUT_OF_INDEX -20
#define FILE_EMPTY_ERROR -30

struct cityDetails
{
    char name[20];
    //To store the name of the city

    int elevation;
    //To store the elevation of the city

    int weatherStatus;
    //City is in danger if weatherStatus=100
    //City is not so safe if weatherStatus=50
    //City is safe if weatherStatus=00

    int population;
    //To hold the current population of the  city

    int capacity;
    //To hold the Maximum population of the  city

    int distance_from_sea;
    //To hold distance from sea to city

    int flag;
    //To see if city is destroyed or not

    int originalIndex;
    //TO keep the old index after sorting

    int med_facility;
    //0-if no medical facility is available
    //1-if medical facility is available
};

struct shortpath
{
    int oriIndex;
    //To keep the original index of the array after sorting

    int distance;
    //To keep the distance value
};

typedef struct cityDetails CITY;//Structure can be used by the name CITY

typedef struct shortpath PATH;

///{^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
///--------------------------------------------GLOBAL VARIABLES-----------------------------------------------------------
CITY cityData[40];//array of structures to hold the details of the cities

CITY TempData[40];//Backup file of cityData

int numofcity; //The number of cities

PATH shortpath[40];

int distance_matrix[40][40];//2d matrix to hold the distance between the cities

int totalPopulation=0;

int graph[40][40];

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~}

///Function name : file_empty_check
//Input parameter: Filename (string)
//Description : It checks the size of the file and returns appropriate value
//Return type : Integer
//Return value : FILE_EMPTY_ERROR or SUCCESS

int file_empty_check(char *filename)
{
    // Declaring "stat" a system call to know about content of file.
    FILE* fp = fopen(filename, "r");

    // checking if the file exist or not
    if (fp == NULL) {
        printf("File Not Found!\n");
        return -1;
    }

    fseek(fp, 0L, SEEK_END);

    // calculating the size of the file
    long int res = ftell(fp);

    // closing the file
    fclose(fp);

    //check size of file and return appropriate status.
    if( res == 0 )
    {
        return FILE_EMPTY_ERROR;
    }
    else
    {
        return SUCCESS;
    }
}

///Function name : Load_matrix
//Input parameter: NULL
//Description : Reads the input matrix from the file
//Return type :INT
//Return value : 1 or -1

int loadmatrix()
{

    // Use a file pointer to open various files to load the values
    FILE *fp;

    //Local variables
    int index       = 0;
    int row_index   = 0;
    int file_status = 0;

    //check whether numofcity.txt file is empty or not.
    file_status = file_empty_check("numofcity.txt");
    if (file_status == -30)
    {
        printf("\nnumofcity file has no content\n");
        return FAILURE;
    }

    //check whether distance_matrix.txt file is empty or not.
    file_status = file_empty_check("distance_matrix.txt");
    if (file_status == -30)
    {
        printf("\ndistance_matrix.txt file has no content\n");
        return FAILURE;
    }

    // Open the numofcity.txt file to read the number of keywords
    fp = fopen("numofcity.txt","r");
    if(fp == NULL)
    {
        printf("\nnumofcity File opening error\n");
        return FAILURE;
    }

    fscanf(fp,"%d", &numofcity);

    if(numofcity <= 0 )
    {
        printf("\nNo. of cities can't be less than 1\n");
        return FAILURE;
    }

    fclose(fp);

    // Open the distance_matrix file to read the number of properties
    fp = fopen("distance_matrix.txt","r");
    if(fp == NULL)
    {
        printf("\nnumofcity File opening error\n");
        return FAILURE;
    }

    // Load the details from file to main memory
    while(!feof(fp)&&row_index<numofcity)
    {
        for(index = 0; index < numofcity; index++)
        {
            fscanf(fp, "%d",&distance_matrix[row_index][index]);
        }
        row_index++;//Increment row index;
    }
    fclose(fp);
    //close the distance_matrix file

    return SUCCESS;
}

///Function name :Load _city_names
//Input parameter:NULL
//Description : loads the city names from the file
//Return type : INT
//Return value : 1 or -1

int load_city_names()
{
     // Use a file pointer to open various files to load the values
    FILE *fp;

    //Local variables
    int row_index   = 0;
    int file_status = 0;

    //check whether nameofcity.txt file is empty or not.
    file_status = file_empty_check("nameofcity.txt");
    if (file_status == -30)
    {
        printf("\nnameofcity file has no content\n");
        return FAILURE;
    }

    // Open the nameofcity.txt file to read the number of keywords
    fp = fopen("nameofcity.txt","r");
    if(fp == NULL)
    {
        printf("\nnameofcity File opening error\n");
        return FAILURE;
    }

    // Load the details from file to main memory
     while(!feof(fp)&&row_index<numofcity)
    {
        fscanf(fp,"%s",cityData[row_index].name);
        fscanf(fp,"%d",&cityData[row_index].population);
        fscanf(fp,"%d",&cityData[row_index].capacity);
        cityData[row_index].originalIndex=row_index;
        cityData[row_index].flag=1;
        totalPopulation+=cityData[row_index].population;
        row_index++;//Increment row index;
    }
    fclose(fp);
    //close the cityData file

    return SUCCESS;
}

 ///Function name :Load_other_info
//Input parameter:NULL
//Description : Loads the other info about cities such as elevation and distance from the sea
//Return type : INT
//Return value : 1 or -1

int load_other_info()
{
     // Use a file pointer to open various files to load the values
    FILE *fp;

    //Local variables
    int row_index   = 0;
    int file_status = 0;

    //check whether city_info.txt file is empty or not.
    file_status = file_empty_check("city_info.txt");
    if (file_status == -30)
    {
        printf("\ncity_info file has no content\n");
        return FAILURE;
    }

    // Open the city_info.txt file to read the number of keywords
    fp = fopen("city_info.txt","r");
    if(fp == NULL)
    {
        printf("\ncity_info File opening error\n");
        return FAILURE;
    }

    // Load the details from file to main memory
     while(!feof(fp)&&row_index<numofcity)
    {
        fscanf(fp,"%d",&cityData[row_index].elevation);

        fscanf(fp,"%d",&cityData[row_index].distance_from_sea);

        row_index++;//Increment row index;
    }
    fclose(fp);
    //close the cityData file

    return SUCCESS;
}

///Function name :Load_weather_status
//Input parameter:NULL
//Description : it lodes the weather status of the cities
//Return type :INT
//Return value : 1 or -1

int load_weather_status()
{
     // Use a file pointer to open various files to load the values
    FILE *fp;

    //Local variables
    int row_index   = 0;
    int file_status = 0;

    //check whether weather_status.txt file is empty or not.
    file_status = file_empty_check("weather_status.txt");

    if (file_status == -30)
    {
        printf("\nweather_status file has no content\n");
        return FAILURE;
    }

    // Open the weather_status.txt file to read the number of keywords
    fp = fopen("weather_status.txt","r");

    if(fp == NULL)
    {
        printf("\nweather_status File opening error\n");
        return FAILURE;
    }

    // Load the details from file to main memory
    while(!feof(fp)&&row_index<numofcity)
    {
        fscanf(fp, "%d", &cityData[row_index].weatherStatus);
        fscanf(fp,"%d",&cityData[row_index].med_facility);
        row_index++;//Increment row index;
    }
    fclose(fp);
    //close the weather_status file

    return SUCCESS;
}

///Function name :dump
//Input parameter:NULL
//Description :It writes file  the updated info of city
//Return type :INT
//Return value :  1 or -1

int dump()
{
    // Local variables
    int spec_index;
    int row_index;

    // File pointer
    FILE *fp;

    // Open the file in write mode and dump the latest key count
    fp = fopen("numofcity.txt","w+");
    if(fp == NULL) {
      printf("\nnumofcity.txt File opening error\n");
        return FAILURE;
    }

    fprintf(fp,"%d", numofcity);//write to file
    fclose(fp);
    //close the numofcity file

    // Open the file in write mode and dump the latest properties count
    fp = fopen("distance_matrix.txt","w+");
    if(fp == NULL)
    {
        printf("\ndistance_matrix.txt File opening error\n");
        return FAILURE;
    }

    for (spec_index = 0; spec_index <= numofcity; spec_index++)
        {
        for (row_index = 0; row_index <= numofcity; row_index++)
        {
            fprintf(fp, "%d ", distance_matrix[spec_index][row_index]);
        }
        fprintf(fp, "%s", "\n");
    }
    fclose(fp);
    //close the distance_matrix file

    //open the file in write mode and dump the latest city details
    fp = fopen("nameofcity.txt","w+");
    if(fp == NULL)
    {
        printf("\nnameof city.txt File opening error\n");
        return FAILURE;
    }

    for (spec_index = 0; spec_index < numofcity; spec_index++)
    {
        fprintf(fp, "%s ", cityData[spec_index].name);
        fprintf(fp, "%d ", cityData[spec_index].population);
        fprintf(fp, "%d ", cityData[spec_index].capacity);
        fprintf(fp, "%s", "\n");

    }
    fclose(fp);
    //close the nameofcity file

    //open the file in write mode and dump the latest weather status
    fp = fopen("weather_status.txt","w+");
    if(fp == NULL)
    {
        printf("\nweather_status.txt File opening error\n");
        return FAILURE;
    }

    for (spec_index = 0; spec_index <= numofcity; spec_index++)
    {
        fprintf(fp, "%d ", cityData[spec_index].weatherStatus);
        fprintf(fp, "%d ", cityData[spec_index].med_facility);
        fprintf(fp, "%s", "\n");
    }
    fclose(fp);
    //close the weather_status file

    return SUCCESS;
}

///Function name :printmatrix
//Input parameter:NULL
//Description : Prints the distance matrix of cities
//Return type :NULL
//Return value :NULL
void printmatrix()
{
    //Empty check
     if(numofcity<=0)
    {
        printf("\nNo data added yet\n");
        return;
    }

    printf("\n");

    for(int i=0;i<numofcity;i++)
    {
        for(int j=0;j<numofcity;j++)
        {
            printf("%4d ",distance_matrix[i][j]);

        }
        printf("\n");
    }
}

///Function name :printcity
//Input parameter:citydata
//Description : Prints the info of city
//Return type : void
//Return value :NULL
void printcity(CITY cityData[])
{
    //Empty check
    if(numofcity<=0)
    {
        printf("\nNo data added yet\n");
        return;
    }

    printf("\nindex  -       City name\tpopulation\tcapacity\tweather\t\televation\tdistance from sea\tMedical facility\n");

    for(int i=0;i<numofcity;i++)
    {
        printf("%d      - %15s\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t\t%d\n",i,cityData[i].name,cityData[i].population,cityData[i].capacity,cityData[i].weatherStatus,cityData[i].elevation,cityData[i].distance_from_sea,cityData[i].med_facility);
    }

    printf("\nNOTE :\n1- medical facility available\n0-no medical facility available\n2-medical facility is supplied by other cities\n");

    printf("\n***weather status****\n0-no danger\n50-moderate danger\n100-most dangerous \n");
}

///Function name :menu1
//Input parameter:NULL
//Description : prints the menu of the program for the user
//Return type : NULL
//Return value : NULL

void menu1()
{
    printf("\n*************MENU****************\n");
    printf("\n404.dump and exit\n");
    printf("1.print matrix\n");
    printf("2.print city details\n");
    printf("3.print duplicate\n");
    printf("4.show the nearest city to Sea \n");
    printf("5.search by city name\n");
    printf("-1.NEXT LEVEL\n");
}

///Function name :menu2
//Input parameter:NULL
//Description : prints the menu of the program for the user
//Return type : NULL
//Return value : NULL

void menu2()
{
    printf("\n*************MENU****************\n");
    printf("\n404.dump and exit\n");
    printf("1.print matrix\n");
    printf("2.print city details\n");
    printf("3.print duplicate\n");
    printf("4.GIVE A CYCLONE ALERT AND Update weather status by weather_status file\n");
    printf("5.search by city name\n");
    printf("6.show the cyclone affected cites in order\n");
    printf("7.sort the cites according to elevation and bad weather\n and evacuate them\n");
    printf("8.show the safe and destroyed city list\n");
    printf("9.Provide the medical facility to affected cities\n");
    printf("10.show the death count\n");
    printf("-1.NEXT LEVEL");
}

///Function name :menu3
//Input parameter:NULL
//Description : prints the menu of the program for the user
//Return type : NULL
//Return value : NULL

void menu3()
{
    printf("\n*************MENU****************\n");
    printf("\n404.dump and exit\n");
    printf("1.print matrix\n");
    printf("2.print city details\n");
    printf("3.print duplicate\n");
    printf("4.search by city name\n");
    printf("5.Show the destroyed cities and recover them\n");
    printf("6.Show the shortest path for officers to visit all the cities\n");
    printf("7.Alternative Solution or precautions to prevent cyclone losses..\n ");
    printf("-1.Exit\n");
}

///Function name : printspecific
//Input parameter: index of city's info
//Description : prints the info of given city
//Return type : NULL
//Return value : NULL

void printspecific(int i)
{
    if(i>=numofcity||i<0)
    {
        printf("\nIndex out of order \n");
        return;
    }

    printf("\nindex  -       City name\tpopulation\tcapacity\tweather\t\televation\tdistance from sea  Medical facility\n");
    printf("%d      - %15s\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t\t%d\n",i,cityData[i].name,cityData[i].population,cityData[i].capacity,cityData[i].weatherStatus,cityData[i].elevation,cityData[i].distance_from_sea,cityData[i].med_facility);

    printf("\nNOTE :\n1- medical facility available\n0-no medical facility available\n2-medical facility is supplied by other cities\n");
    printf("\n***weather status****\n0-no danger\n50-moderate danger\n100-most dangerous \n");

}

///Function name :printonlycity
//Input parameter: city data
//Description : gives the description of the ciy
//Return type : NULL
//Return value : NULL

void printonlycity(CITY cityData[])
{
    //Empty check
    if(numofcity<=0)
    {
        printf("\nNo data added yet\n");
        return;
    }

    printf("\nindex  -       City name\tdistance from sea\n");
    for(int i=0;i<numofcity;i++)
    {
        printf("%d      - %15s\t%d\n",i,cityData[i].name,cityData[i].distance_from_sea);
    }
}

void printonlycity2(CITY cityData[])
{
    //Empty check
    if(numofcity<=0)
    {
        printf("\nNo data added yet\n");
        return;
    }

    printf("\nindex  -       City name\tElevation\n");
    for(int i=0;i<numofcity;i++)
    {
        printf("%d      - %15s\t%d\n",i,cityData[i].name,cityData[i].elevation);
    }
}

///Function name :search
//Input parameter: name of city
//Description : searchs the city in the file
//Return type : INT
//Return value : 1 or -1

int search(char s[])
{
    int i;

    for(i=0;i<numofcity;i++)
    {
        //comparing given string to city names
        if(strcmp(cityData[i].name,s)==0)
          return i;//return index of city
    }
    return FAILURE;
}

///Function name :dijkstras
//Input parameter:number of cities and source city
//Description :to find the shortest distance from the source city to every
//              other cities
//Return type : void
//Return value : NULL

void dijkstras(int v,int source)
{
    ///Local variable
   int visited[40];
   int queue[40];
   int i;
   int j;
   int front=0;
   int rear=0;
   int u;
   int sum;

   //initializing visited array and shortpath distance
   for(i=0;i<=v;i++)
   {
       visited[i]=0;
       shortpath[i].distance=INT_MAX;
       shortpath[i].oriIndex=i;
   }

   visited[source]=1;
   queue[rear]=source;
   shortpath[source].distance=0;

   for(i=0;i<v;i++)
   {
       u=queue[front];
       front++;

       for(j=0;j<v;j++)
       {
           if(distance_matrix[u][j]!=0)
           {
               if(visited[j]==0)
               {
                   rear++;
                   queue[rear]=j;
               }
               sum=shortpath[u].distance+distance_matrix[u][j];

               if(sum<shortpath[j].distance)
               {
                   shortpath[j].distance=sum;
;
               }
               visited[u]=1;//mark the city as visited
           }
       }
   }

}

///Function name :Merge2
//Input parameter: distance from sea
//Description : sorts the cities as per the distance from the sea.
//Return type :NULL
//Return value :NULL

void Merge2(PATH b[50],int p,PATH c[50],int q,PATH a[50])
{
    int i=0,j=0,k=0;

    while(i<p&&j<q)
    {
        if(b[i].distance<=c[j].distance)
        {
            a[k]=b[i];
            i++;
        }
        else
        {
          a[k]=c[j];
          j++;
        }
        k++;
    }
    if(i==p)
    {

        for(i=j;i<q;i++)
        {
            a[k]=c[i];
            k++;
        }
    }
    else
        for(j=i;j<p;j++)
        {
            a[k]=b[j];
            k++;
        }

}

///Function name :MergeSort2
//Input parameter: distance from sea
//Description : sorts the cities as per the distance from the sea.
//Return type :NULL
//Return value :NULL

void MergeSort2(PATH a[50],int n)
{
    if(n>1)
    {
      int i;
      PATH b[100],c[100];

      for(i=0;i<(n/2);i++)
      {
        b[i]=a[i];
      }

      int k=0;

      for(i=n/2;i<n;i++)
      {
        c[k]=a[i];
        k++;
      }

      MergeSort2(b,n/2);
      MergeSort2(c,k);
      Merge2(b,n/2,c,k,a);
    }

}

///Function name :movetosafe
//Input parameter:an integer number
//Description :This function basically moves the population of cites which are in danger zone to a safe city
//Return type :integer
//Return value :1 or -1

int movetosafe(int num)
{
    int j=0,i ;
    if(num>=numofcity)
        return FAILURE;

    while(j != num)
    {
        dijkstras(numofcity,TempData[j].originalIndex);

        MergeSort2(shortpath,numofcity);

        for(i = 1;i < numofcity&&TempData[j].population;i++)
        {

           if(TempData[cityData[shortpath[i].oriIndex].originalIndex].flag&&TempData[cityData[shortpath[i].oriIndex].originalIndex].population<TempData[cityData[shortpath[i].oriIndex].originalIndex].capacity)
           {

               TempData[cityData[shortpath[i].oriIndex].originalIndex].population+=TempData[j].population;

               if(TempData[cityData[shortpath[i].oriIndex].originalIndex].population>TempData[cityData[shortpath[i].oriIndex].originalIndex].capacity)
               {
                   TempData[j].population=TempData[cityData[shortpath[i].oriIndex].originalIndex].population-TempData[cityData[shortpath[i].oriIndex].originalIndex].capacity;
                   TempData[cityData[shortpath[i].oriIndex].originalIndex].population=TempData[cityData[shortpath[i].oriIndex].originalIndex].capacity;
               }
               else
               {
                  TempData[j].population=0;
                  TempData[j].flag=0;
               }
           }
        }
        if(i >= numofcity)
        {
            TempData[j].flag=0;
           return FAILURE;
        }
       j++;
    }

    return SUCCESS;
}

///Function name :Merge1
//Input parameter:City name
//Description :sorts the city as per the city name
//Return type :NULL
//Return value :NULL

void Merge1(CITY b[50],int p,CITY c[50],int q,CITY a[50])
{
    int i=0,j=0,k=0;

    while(i<p&&j<q)
    {
        if(b[i].distance_from_sea<=c[j].distance_from_sea)
        {
            a[k]=b[i];
            i++;
        }
        else
        {
          a[k]=c[j];
          j++;
        }

        k++;
    }

    if(i==p)
    {

        for(i=j;i<q;i++)
        {
            a[k]=c[i];
            k++;
        }
    }
    else
        for(j=i;j<p;j++)
        {
            a[k]=b[j];
            k++;
        }

}

///Function name :MergeSort1
//Input parameter:City name
//Description :sorts the city as per the city name
//Return type :NULL
//Return value :NULL

void MergeSort1(CITY a[50],int n)
{

    if(n>1)
    {

      int i;
      CITY b[100],c[100];

     for(i=0;i<(n/2);i++)
     {
        b[i]=a[i];
     }

     int k=0;

     for(i=n/2;i<n;i++)
     {
        c[k]=a[i];
       k++;
     }

     MergeSort1(b,n/2);
     MergeSort1(c,k);
     Merge1(b,n/2,c,k,a);
    }

}

int partition(CITY a[],int x,int y)
{
    //Local Variables
    CITY temp;
    int p=a[x].weatherStatus;
    int i=x;
    int j=y+1;

    do
    {
       do
        {
            i=i+1;
        }while(((a[i].weatherStatus)>(p))&&(i<numofcity));

       do
        {
            j=j-1;
        } while((a[j].weatherStatus)<(p)&&j!=-1);

        temp=a[i];
        a[i]=a[j];
        a[j]=temp;

    }while(i<j);

    temp=a[i];
    a[i]=a[j];
    a[j]=temp;

    temp=a[x];
    a[x]=a[j];
    a[j]=temp;

    return j;

}

///Function name :quickSort
//Input parameter:City name
//Description :This function sorts the city based o their city names
//Return type :NULL
//Return value :NULL

void quickSort(CITY a[],int l,int r)
{
    if(l<r)
    {
        int  s=partition(a,l,r);
        quickSort(a,l,s-1);
        quickSort(a,s+1,r);

    }
}

///Function name :bad_weather
//Input parameter: city name
//Description : sorts the cities as per the weather condition
//Return type : NULL
//Return value : NULL

void bad_weather(CITY city[])
{
    quickSort(city,0,numofcity-1);
    printcity(city);

}

///Function name :update_Original
//Input parameter:NULL
//Description : updates the original info of city
//Return type :NULL
//Return value :NULL

void update_Original()
{
    for(int i=0;i<numofcity;i++)
    {
        cityData[TempData[i].originalIndex].flag=TempData[i].flag;
        cityData[TempData[i].originalIndex].population=TempData[i].population;
        cityData[i].originalIndex=TempData[cityData[i].originalIndex].originalIndex;
    }
}

///Function name :update_temp
//Input parameter:NULL
//Description :This function updates the med facility and weather status
//Return type :NULL
//Return value :NULL

void update_temp()
{
    for(int i=0;i<numofcity;i++)
    {
        TempData[TempData[i].originalIndex].weatherStatus=cityData[i].weatherStatus;
        TempData[TempData[i].originalIndex].med_facility=cityData[i].med_facility;
    }
}

///Function name :check_for_death
//Input parameter:NULL
//Description :This function counts the death during the calamity
//Return type :NULL
//Return value :NULL

void check_for_death()
{
    int sum=0;
    for(int i=0;i<numofcity;i++)
    {
        sum+=cityData[i].population;
    }
    if(sum==totalPopulation)
    {
        printf("\nDue to Proper precaution and planning no death is happened till now!!!!\n");
    }
    else
        printf("\nDeath count till now = %d \n",totalPopulation-sum);
}

///Function name :show_safe_destroyed
//Input parameter:NULL
//Description :This function lists the cities which are destroyed and yet not destroyed during the calamity
//Return type :NULL
//Return value :NULL

void show_safe_destroyed()
{
    printf("\nDestroyed city list\n");
    for(int i=0;i<numofcity;i++)
    {
        if(cityData[i].flag==0)
        {
            puts(cityData[i].name);
        }
    }
     printf("\nNOT YET Destroyed city list\n");
    for(int i=0;i<numofcity;i++)
    {
        if(cityData[i].flag==1)
        {
            puts(cityData[i].name);
        }
    }
}

///Function name :show_destroyed_cities
//Input parameter:NULL
//Description :This function lists the cities which are destroyed and yet not destroyed during the calamity
//Return type :NULL
//Return value :NULL

void show_destroyed_cities()
{
    printf("\nDestroyed city list\n");
    for(int i=0;i<numofcity;i++)
    {
        if(cityData[i].flag==0)
        {
            puts(cityData[i].name);
            cityData[i].flag=1;
        }
    }

}

///Function name :minKey
//Input parameter:key element and vertex
//Description : A utility function to find the vertex with
//              minimum key value, from the set of vertices
//              not yet included in MST
//Return type :integer
//Return value :min_index

int minKey(int key[], bool mstSet[],int V)
{
    // Initialize min value
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++)
        if (mstSet[v] == false && key[v] < min)
            min = key[v], min_index = v;

    return min_index;
}

///Function name :printMST
//Input parameter:A 2D matrix  and vertex and a parent matrix
//Description : A utility function to print the
//              constructed MST stored in parent[]
//Return type :NULL
//Return value :NULL

void printMST(int parent[40], int graph[40][40],int V)
{
    printf("\n      Route \t\tWeight\n");
    for (int i = 1; i < V; i++)
        printf(" %8s  - %8s\t%d \n",cityData[parent[i]].name,cityData[i].name ,graph[i][parent[i]]);
}

//
 ///Function name :primMST
//Input parameter: A 2D matrix  and vertex
//Description :Function to construct and print MST for
//             a graph represented using adjacency
//              matrix representation
//Return type :NULL
//Return value :NULL

void primMST(int graph[40][40],int V)
{
    // Array to store constructed MST
    int parent[V];
    // Key values used to pick minimum weight edge in cut
    int key[V];
    // To represent set of vertices not yet included in MST
    bool mstSet[V];

    // Initialize all keys as INFINITE
    for (int i = 0; i < V; i++)
        key[i] = INT_MAX, mstSet[i] = false;

    // Always include first 1st vertex in MST.
    // Make key 0 so that this vertex is picked as first vertex.
    key[0] = 0;
    parent[0] = -1; // First node is always root of MST

    // The MST will have V vertices
    for (int count = 0; count < V - 1; count++) {
        // Pick the minimum key vertex from the
        // set of vertices not yet included in MST
        int u = minKey(key, mstSet,V);

        // Add the picked vertex to the MST Set
        mstSet[u] = true;

        // Update key value and parent index of
        // the adjacent vertices of the picked vertex.
        // Consider only those vertices which are not
        // yet included in MST
        for (int v = 0; v < V; v++)

            // graph[u][v] is non zero only for adjacent vertices of m
            // mstSet[v] is false for vertices not yet included in MST
            // Update the key only if graph[u][v] is smaller than key[v]
            if (graph[u][v] && mstSet[v] == false && graph[u][v] < key[v])
                parent[v] = u, key[v] = graph[u][v];
    }

    // print the constructed MST
    printMST(parent, graph,V);
}

///Function name :floyds
//Input parameter:The matrix (in 2D format)
//Description :This function finds all pair of shortest path
//Return type :NULL
//Return value :NULL

void floyds(int  m[40][40],int n)
{
    for(int k=0;k<n;k++)
    {
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
            {
                if(m[i][j]>m[i][k]+m[k][j])
                {
                   m[i][j]=m[i][k]+m[k][j];
                }
            }
        }
    }
}

///Function name :medication
//Input parameter: 2d matrix
//Description : Provides medicals to other cities if it has people and not destroyed
//              one city can provide medicals at most 6 other cities
//              city provides medicals to other cities according to shortest distance
//Return type : void
//Return value : NULL

void medication(int graph[40][40])
{
    //using floyd's  algorithm to find shortest path
    floyds(graph,numofcity);

    for(int i=0;i<numofcity;i++)
    {
        if(cityData[i].med_facility==1&&cityData[i].flag==1)
        {
           for(int j=0;j<numofcity;j++)
           {
               shortpath[j].distance=graph[i][j];
               shortpath[j].oriIndex=j;
           }

           MergeSort2(shortpath,numofcity);
            printf("\n%s provides medical facility to \n",cityData[i].name);
            int k=0;
            for(int j=1;j<numofcity&&k<=6;j++)
            {
                if(i!=shortpath[j].oriIndex&&!cityData[shortpath[j].oriIndex].med_facility&&cityData[shortpath[j].oriIndex].flag)
                {
                   puts(cityData[shortpath[j].oriIndex].name);
                   cityData[shortpath[j].oriIndex].med_facility=2;
                   k++;
                }

            }
            if(k==0)
            {
              printf("No city!!\n");
            }
        }
    }

}

///Function name :Merge3
//Input parameter:City names
//Description :sorts the city as per the city elevation
//Return type :NULL
//Return value :NULL

void Merge3(CITY b[50],int p,CITY c[50],int q,CITY a[50])
{
    int i=0,j=0,k=0;

    while(i<p&&j<q)
    {
        if(b[i].elevation<=c[j].elevation)
        {
            a[k]=b[i];
            i++;
        }
        else
        {
          a[k]=c[j];
          j++;
        }
        k++;
    }
    if(i==p)
    {

        for(i=j;i<q;i++)
        {
            a[k]=c[i];
            k++;
        }
    }
    else
        for(j=i;j<p;j++)
        {
            a[k]=b[j];
            k++;
        }

}


///Function name :MergeSort3
//Input parameter:City names
//Description :sorts the city as per the city name
//Return type :NULL
//Return value :NULL

void MergeSort3(CITY a[50],int n)
{
    if(n>1)
    {
      int i;

      CITY b[100],c[100];

     for(i=0;i<(n/2);i++)
     {
        b[i]=a[i];
     }

     int k=0;

     for(i=n/2;i<n;i++)
     {
        c[k]=a[i];
       k++;
     }

     MergeSort3(b,n/2);
     MergeSort3(c,k);
     Merge3(b,n/2,c,k,a);
    }

}

///Function name :elevation_sort
//Input parameter:NULL
//Description :This checks the possibility of destruction which are at low elevation and having
                //bad weather condition and it will move the cities which are at high alert to
                //a safe place.
//Return type :NULL
//Return value :NULL

void solutions()
{
    printf("\nThings to do before storm makes a landfall/ destruction \n\n1) First of all there is no need to panic. People should stay calm and must ignore rumours.\n2) Keep your mobiles charged to stay abreast with the latest information about the cyclone. Also stay updates on radio and TV.\n3) Keep a small emergency kit ready in case of any emergency arising out of any injury.\n4) Keep your important documents in water proof packet or container.\n5) Secure your house properly and don't keep sharp objects loose lest they cause injury during heavy wind.\n\n\n");

    printf("Things to keep in mind during cyclone\n\n1) Switch off electrical equipment and mainline. Also close gas connections.\n2) Keep the doors and windows of your house shut as outside objects may hit the house in heavy winds.\n3) If you think your house may not withstand the fiery winds, then leave before the onset of cyclone.\n4) Always remember to drink chlorinated or boiled water to avoid any water born diseases.\n5) Only rely on official information and warnings, avoid rumours and fake news.\n\n\n");

    printf("Things to keep in mind after cyclone\n\n1) Avoid entering damaged building as they may have suffered during the storm.\n2) Do watch out for broken electric poles, sharp objects and sharp objects.\n3) Seek a safe refuge soon after the cyclone passes.\n");



}

///Function name :elevation_sort
//Input parameter:NULL
//Description :This checks the possibility of destruction which are at low elevation and having
                //bad weather condition and it will move the cities which are at high alert to
                //a safe place.
//Return type :NULL
//Return value :NULL

void elevation_sort()
{
    int num,choice;
    MergeSort3(TempData,numofcity);

    printonlycity2(TempData);
    printf("Cities with low elevation and bad weather has high possibility of destruction!!!!!\n");

    printf("\nDo you want to move the cities to nearest safe Cities ??\nEnter 1 for YES or 2 for NO\n");
    scanf("%d",&choice);

    if(choice == 1)
   {
       printf("\nEnter the number of cities you want to move\n");
       scanf("%d",&num);
       int status = movetosafe(num);
       if(status == SUCCESS)
         printf("\nMoved Successfully\n");
       else
         printf("\nMoving process was not Successful\n");
    }
     update_Original();


}


int main()
{
   int status;
   char choice[10];
   int num,ch;
   char string[20];

   status=loadmatrix();
   if(status==FAILURE)
   {
      printf("\nFailure\n");
      return 0;
   }

   status=load_city_names();
   if(status==FAILURE)
   {
      printf("\nFailure\n");
      return 0;
   }

   status=load_other_info();

   if(status==FAILURE)
   {
      printf("\nFailure\n");
      return 0;
   }

   memcpy(&TempData, &cityData, sizeof(cityData));
   memcpy(&graph, &distance_matrix, sizeof(distance_matrix));
   printf("\n*************************************************************************************************************************\n");
   printf("                                      WELCOME TO PROJECT-GYRATE\n");
   printf("                            Cyclone Disaster Management And Prevention Program\n");
   printf("*************************************************************************************************************************\n");
   printf("\nFIRST PHASE!!! PRECAUTION LEVEl!!!!!!!!!\n");
   while(ch!=-1)
   {

        menu1();
        printf("\nEnter your choice\n");
        scanf("%s",choice);
        ch=atoi(choice);
        switch(ch)
       {
          case 404:
            status = dump();
            if(status==FAILURE)
                 printf("\nFailure\n");
            else
                 printf("\nDumped SuccessFully!!!!\n");
            return 0;

          case 1:
            printmatrix();
            break;

          case 2:
            printcity(cityData);
            break;

          case 3:
            printcity(TempData);
            break;

          case 4:
            MergeSort1(TempData,numofcity);
            printonlycity(TempData);
            update_Original();
            printf("\nDo you want to move the nearest cities to safe Cities as precaution??\nEnter 1 for YES or 2 for NO\n");
            scanf("%d",&ch);

            if(ch == 1)
            {
               printf("\nEnter the number of cities you want to move\n");
               scanf("%d",&num);
               status = movetosafe(num);
               if(status == SUCCESS)
                 printf("\nMoved Successfully\n");
               else
                 printf("\nMoving process was not Successful\n");
            }
            update_Original();
            break;

          case 5:
              printf("\nEnter city name \n");
              scanf("%s",string);
              status = search(string);
              if(status == FAILURE)
              {
                  printf("\nCity not found \n");
                  break;
              }
              printspecific(status);
              break;
          case -1:
             break;
          default :
            printf("\nEnter valid input\n");
       }
   }


   ch=0;
   printf("\nSECOND PHASE !!!!!CYCLONE ATTACK!!!!\n");
   while(ch!=-1)
   {

        menu2();
        printf("\nEnter your choice\n");
        scanf("%s",choice);
        ch=atoi(choice);
        switch(ch)
       {
          case 404:
            status=dump();
            if(status==FAILURE)
                 printf("\nFailure\n");
            else
                 printf("\nDumped SuccessFully!!!!\n");
            return 0;

          case 1:
            printmatrix();
            break;

          case 2:
            printcity(cityData);
            break;

          case 3:
            printcity(TempData);
            break;

          case 4:
              status = load_weather_status();
              printf("\nCYCLONE ATTACK ON THE CITIES HAVE BEEN STARTED !!!!!!!!!!!!!!!!!\n");
              printf("\nALERT!!!!!!!");
              update_temp();
            if(status == FAILURE)
              printf("\nFailure\n");
            else
                printf("\nWEATHER STATUS HAS BEEN UPDATED!!!!!!!!!!!\n");
              break;

          case 5:
              printf("\nEnter city name \n");
              scanf("%s",string);
              status = search(string);
              if(status == FAILURE)
              {
                  printf("\nCity not found \n");
                  break;
              }
              printspecific(status);
              break;

          case 6:
              bad_weather(TempData);
              break;

          case 7:
              elevation_sort();
              break;

          case 8:
              show_safe_destroyed();
              break;

          case 9 :
               medication(graph);
              break;

          case 10:
              check_for_death();
              break;
          case -1:
             break;
          default :
            printf("\nEnter valid input\n");
        }
    }
    printf("\nTHIRD PHASE !!!!!CITY RECOVERY!!!!\n");
     while(1)
   {

        menu3();
        printf("\nEnter your choice\n");
        scanf("%s",choice);
        ch=atoi(choice);
        switch(ch)
       {
          case 404:
            status=dump();
            if(status==FAILURE)
                 printf("\nFailure\n");
            else
                 printf("\nDumped SuccessFully!!!!\n");
            return 0;

          case 1:
            printmatrix();
            break;

          case 2:
            printcity(cityData);
            break;

          case 3:
            printcity(TempData);
            break;

          case 4:
              printf("\nEnter city name \n");
              scanf("%s",string);
              status = search(string);
              if(status == FAILURE)
              {
                  printf("\nCity not found \n");
                  break;
              }
              printspecific(status);
              break;

          case 5:
              show_destroyed_cities();
              printf("\nDestroyed cities are successfully recovered\n");
              break;

          case 6:
              primMST(graph,numofcity);
              break;

          case 7 :
            solutions();
            break;
          case -1:
              {
                     printf("\n*************************************************************************************************************************\n");
                     printf("                            GIVE US FEEDBACK ABOUT THE PROJECT\n");
                     printf("                            THANK YOU FOR USING\n");
                     printf("                            PROGRAM TERMINATING................:)\n");
                     printf("***************************************************************************************************************************\n");
                  return 0;
              }
          default :
            printf("\nEnter valid input\n");

        }
    }


    return 0;
}
