/*
    Author: Yash Vasani
    Title: SUPER_MARKET_SYSTEM 
*/
#include<iostream>
#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<iomanip>

#define SIZE 17    //Size of the HashTable, if the datasets which are used, are big then SIZE is set to the sufficient high value,
                   //so as the searching would be fast, otherwise it's set to moderately normal value.

using namespace std;

//////////////////// PRODUCT NODE ////////////////////
struct PRODUCT
{
    long long Product_ID;
    string Name;
    float Rate;
    int Quantity;
    PRODUCT *next;
};

//////////////////// CUSTOMER NODE ////////////////////
struct CUSTOMER
{
    long long Customer_ID;
    string Name;
    float Points;
    CUSTOMER *next;
};

//////////////////// BILL NODE ////////////////////
struct BILL
{
    long long Product_ID;
    string Name;
    float Rate;
    int Quantity;
    float Amount;
    BILL *next;
};

//////////////////// CLASS DRIVE ////////////////////
class DRIVE //I have renamed Support_Library class to DRIVE class.
{
    public:
    // F(X) which converts num to string
    string num_to_str(long long num);

    // for Printing lines
    void line();
};

string DRIVE::num_to_str(long long num)
{
    int i=0,length=floor(log10(num))+1;
    char str[length+1];

    while(num)
    {
        str[length-1-i++]=num%10 + 48;
        num/=10;
    }
    str[length]='\0';

    return str;
}

void DRIVE::line()
{
    for(int i=0;i<63;++i)
        printf("_");
}

//////////////////// CLASS Linked_List ////////////////////
template<typename node>
class Linked_List
{
    node *head,*tail;

    public:
        Linked_List(){ head=NULL; tail=NULL; }

        //Getter-Setter for head
        node* getHead(){ return head; }
        void setHead(node *ptr){ head=ptr; }

        //Getter-Setter for tail
        node* getTail(){ return tail; }
        void setTail(node *ptr){ tail=ptr; }

        //NOTE: I haven't implemented here the insert_node or the delete_node F(x) because this linked list is used for the both
        //Product_database and Customer_database and they have different parameters. Insted I have done separately in the
        //corresponding class.
};

//////////////////// CLASS INVENTORY ////////////////////
class INVENTORY:public virtual DRIVE
{
    //We have made the Product_database static so that all the managers would get to know if any change is made
    //and for the entire system this database remains same (synchronized).
    static Linked_List<PRODUCT> *Product_database;


    ////////// These two F(x)s are accessible by the both the MANAGER and BILLING class object. //////////
    protected:

        //change is for how much increament or decreament we want to update.
        void Update_Stock(long long n,int change);

        //We are retrieving Product_Data by ptr.
        //It returns the const ptr of the product having ID n, so it won't be changed as it's read only variable,
        //by the BILLING class object.
        inline const PRODUCT* RETRIEVE_Product_Data(long long n);


    ////////// These F(x)s are only accesible by the MANAGER class object. //////////
    private:
        void Add_Product(long long n, string t, float rt, int qua);

        void Delete_Product(long long n);

        //It returns the ptr of the product having ID n, so it could be changed , by the MANAGER class object.
        PRODUCT* Search_Product(long long n);

        //We are assuming that ID wouldn't be changed.
        void Update_Product(long long n, float rt, int qua);

        void Display_Product_Data(long long n);

        void Display_Product_database();

        friend class MANAGER;
};

//DEFINITION OF STATIC DATA MEMBER
Linked_List<PRODUCT>* INVENTORY:: Product_database= new Linked_List<PRODUCT>[SIZE];;

void INVENTORY::Add_Product(long long n, string t, float rt, int qua)
{
    int index=n%SIZE;   //hash_function

    PRODUCT *Head=Product_database[index].getHead();
    PRODUCT *tmp=new PRODUCT;
    tmp->Product_ID=n;
    tmp->Name=t;
    tmp->Rate=rt;
    tmp->Quantity=qua;
    tmp->next=NULL;

    if(Head==NULL)
    {
        Product_database[index].setHead(tmp);
        Product_database[index].setTail(tmp);
    }
    else
    {
        PRODUCT *ptr=Head, *ptr1=NULL;
        while(ptr!=NULL && (ptr->Product_ID)<n)
        {
            ptr1=ptr;
            ptr=ptr->next;
        }

        if(ptr==Head)//if tmp would be the first element of the list.
        {
            Product_database[index].setHead(tmp);
            tmp->next=ptr;
        }
        else if(ptr==NULL)//if the tmp would be the last element of the list.
        {
            ptr1->next=tmp;
            Product_database[index].setTail(tmp);
        }
        else//other than corner cases.
        {
            tmp->next=ptr;
            ptr1->next=tmp;
        }
    }

}

PRODUCT* INVENTORY::Search_Product(long long n)
{
    int index=n%SIZE;   //hash_function

    PRODUCT *ptr=Product_database[index].getHead();

    while(ptr!=NULL && (ptr->Product_ID)!=n)
        ptr=ptr->next;

    return ptr;
}

void INVENTORY::Delete_Product(long long n)
{
    int index=n%SIZE;   //hash_function
    PRODUCT *Head=Product_database[index].getHead(), *Tail=Product_database[index].getTail();
    cout<<Head<<endl<<Tail<<endl;
    PRODUCT *curr=Head,*pre=NULL;

    if(n==Head->Product_ID)       //here curr is at head
    {
        if(n==Tail->Product_ID)    //we are checking that whether list had only one element or not.
            Product_database[index].setTail(NULL);

        Product_database[index].setHead(Head->next);
        delete curr;    //we are removing previous head after deletion it won't be needed
    }
    else if(n==Tail->Product_ID)
    {
        while(curr!=Tail)
        {
            pre=curr;
            curr=curr->next;
        }

        pre->next=NULL;
        Product_database[index].setTail(pre);
        delete curr;    //we are removing previous tail after deletion it won't be needed
    }
    else
    {
        while(curr!=NULL)
        {
            if(curr->Product_ID==n)
            {
                pre->next=curr->next;
                delete curr;
                break;
            }
            else
                pre=curr;

            curr=curr->next;
        }
    }
}

void INVENTORY::Update_Product(long long n, float rt, int qua)
{

    PRODUCT *ptr=Search_Product(n);

    if(rt!=ptr->Rate)//If Rate is changed.
        ptr->Rate=rt;

    if(qua!=ptr->Quantity)//If Qunatity is changed.
       ptr->Rate=qua;
}

void INVENTORY::Update_Stock(long long n, int change)
{
    PRODUCT *ptr=Search_Product(n);

    ptr->Quantity=ptr->Quantity + change;
}

inline const PRODUCT* INVENTORY::RETRIEVE_Product_Data(long long n)
{
    const PRODUCT *ptr=Search_Product(n);

    return ptr;
}

void INVENTORY::Display_Product_database()
{
    for(int index=0;index<SIZE;++index)
    {
        cout<<"INDEX NO. "<<index;

        PRODUCT *ptr=Product_database[index].getHead();
        cout<<ptr<<endl;
        while(ptr!=NULL)
        {
            cout<<"-->"<<'['<<ptr->Product_ID<<" : "<<ptr->Name<<" : "<<ptr->Rate<<" : "<<ptr->Quantity<<']';
            ptr=ptr->next;
        }
        cout<<endl;
    }
}

void INVENTORY::Display_Product_Data(long long n)
{
    PRODUCT *ptr=Search_Product(n);

    cout<<ptr->Product_ID<<"\t";
    cout<<setw(5)<<ptr->Name<<"\t";
    cout<<setw(13)<<fixed<<setprecision(2)<<ptr->Rate<<"\t";
    cout<<setw(9)<<ptr->Quantity<<"\t\n";
}

//////////////////// CLASS MANAGER ////////////////////
class MANAGER:public INVENTORY
{
    long long Manager_ID;
    string Name;

    //NOTE: These F(x)s are only calling from main, because corresponding INVENTORY class' inherited private F(x)s.
    public:

        MANAGER(long long n, string t){ Manager_ID=n; Name=t; }

        inline void Display_Manager_Data()
        { cout<<"\n\nMANAGER ID : "<<setw(10)<<Manager_ID<<"\nNAME : "<<setw(10)<<Name<<"\n\n"; }

        inline void AddProduct(long long n, string t, float rt, int qua){ Add_Product(n, t, rt, qua); }

        inline void DeleteProduct(long long n){ Delete_Product(n); }

        inline PRODUCT* SearchProduct(long long n){ return Search_Product(n); }

        //We are assuming that ID wouldn't be changed.
        inline void UpdateProduct(long long n, float rt, int qua){ Update_Product(n, rt, qua); }

        //change is for how much increament or decreament we want to update.
        inline void UpdateStock(long long n,int change){ Update_Stock(n, change); }

        inline void DisplayProductData(long long n){ Display_Product_Data(n); }

        inline void DisplayProductdatabase(){ Display_Product_database(); }
};

//////////////////// CLASS CUSTOMER_CENTER ////////////////////
class CUSTOMER_CENTER:public virtual DRIVE
{
    static Linked_List<CUSTOMER> *Customer_database;

    public:

        void Add_Customer(long long n, string t, float pts);

        CUSTOMER* Search_Customer(long long n);

        void Update_Points(long long n,int change);

        void Display_Customer_database();

};

//DEFINITION OF STATIC DATA MEMBER
Linked_List<CUSTOMER>* CUSTOMER_CENTER:: Customer_database=new Linked_List<CUSTOMER>[SIZE];

void CUSTOMER_CENTER::Add_Customer(long long n, string t, float pts)
{
    int index=n%SIZE;   //hash_function

    CUSTOMER *Head=Customer_database[index].getHead();
    CUSTOMER *tmp=new CUSTOMER;
    tmp->Customer_ID=n;
    tmp->Name=t;
    tmp->Points=pts;
    tmp->next=NULL;

    if(Head==NULL)
    {
        Customer_database[index].setHead(tmp);
        Customer_database[index].setTail(tmp);
    }
    else
    {
        CUSTOMER *ptr=Head, *ptr1=NULL;
        while(ptr!=NULL && (ptr->Customer_ID)<n)
        {
            ptr1=ptr;
            ptr=ptr->next;
        }

        if(ptr==Head)//if tmp would be the first element of the list.
        {
            Customer_database[index].setHead(tmp);
            tmp->next=ptr;
        }
        else if(ptr==NULL)//if the tmp would be the last element of the list.
        {
            ptr1->next=tmp;
            Customer_database[index].setTail(tmp);
        }
        else//other than corner cases.
        {
            tmp->next=ptr;
            ptr1->next=tmp;
        }
    }
}

CUSTOMER* CUSTOMER_CENTER::Search_Customer(long long n)
{
    int index=n%SIZE;   //hash_function

    CUSTOMER* ptr=Customer_database[index].getHead();

    while(ptr!=NULL && ptr->Customer_ID!=n)
        ptr=ptr->next;

    if(ptr!=NULL)
        return ptr;
    else
        return NULL;
}

void CUSTOMER_CENTER::Update_Points(long long n, int change)
{

    CUSTOMER* ptr=Search_Customer(n);

    ptr->Points=ptr->Points + change;
}

void CUSTOMER_CENTER::Display_Customer_database()
{
    for(int index=0;index<SIZE;++index)
    {
        cout<<"INDEX NO. "<<index;

        CUSTOMER *ptr=Customer_database[index].getHead();

        while(ptr!=NULL)
        {
            cout<<"-->"<<'['<<ptr->Customer_ID<<" : "<<ptr->Name<<" : "<<ptr->Points<<']';
            ptr=ptr->next;
        }
        cout<<endl;
    }
}

//////////////////// CLASS BILLING ////////////////////
class BILLING:public CUSTOMER_CENTER, public INVENTORY
{
    long long Transaction_ID;// I am gerating integer Transaction_ID randomly by the rand() F(x).
    long long Customer_ID;
    //NOTE : This list is the list of buying products of corresponding customer, We don't need to make it hashtable
    //because we have to just traverse all the products that are bought by the cusotmer for making payment.

    Linked_List<BILL> Product_list;

    float Total_Amount;

    public:
        BILLING(){ Total_Amount=0; }
        //It just inserts the product having ID n into the Linked_List named Product_list.
        void Buy_Product(long long n,int qua);

        //It just Delets the product having ID n into the Linked_List named Product_list.
        void Remove_Product(long long n);

        inline void Assign_Customer_ID(long long n);

        void Make_Payment();

        //After the one customer's shopping episode Total_amount, Product_list etc
        //have to be reinitialized.
        void Restore_BILLING_Data();
};

void BILLING::Buy_Product(long long n,int qua)
{
    BILL *Head=Product_list.getHead();
    BILL *tmp=new BILL;

    const PRODUCT *pdt=RETRIEVE_Product_Data(n);

    tmp->Product_ID=n;
    tmp->Name=pdt->Name;
    tmp->Rate=pdt->Rate;
    tmp->Quantity=qua;
    tmp->Amount=qua*(pdt->Rate);
    tmp->next=NULL;

    Total_Amount+=tmp->Amount;
    Update_Stock(n,-qua); //BILLING agent has to reduce the product stock by qua.
    //Adding the node into the Linked_List named Product_list.
    if(Head==NULL)
    {
        Product_list.setHead(tmp);
        Product_list.setTail(tmp);
    }
    else
    {
        BILL *ptr=Head, *ptr1=NULL;
        while(ptr!=NULL && (ptr->Product_ID)<n)
        {
            ptr1=ptr;
            ptr=ptr->next;
        }

        if(ptr==Head)//if tmp would be the first element of the list.
        {
            Product_list.setHead(tmp);
            tmp->next=ptr;
        }
        else if(ptr==NULL)//if the tmp would be the last element of the list.
        {
            ptr1->next=tmp;
            Product_list.setTail(tmp);
        }
        else//other than corner cases.
        {
            tmp->next=ptr;
            ptr1->next=tmp;
        }
    }
}

void BILLING::Remove_Product(long long n)
{
    BILL *Head=Product_list.getHead(), *Tail=Product_list.getTail();
    BILL *curr=Head,*pre=NULL;

    if(n==Head->Product_ID)       //here curr is at head
    {
        if(n==Tail->Product_ID)    //We are checking whether list had only one element or not.
            Product_list.setTail(NULL);

        Product_list.setHead(Head->next);
        Total_Amount-=Head->Amount;//We have to remove corrsponding Amount from the Total_Amount, as this product is no longer.
        Update_Stock(n,Head->Quantity);//BILLING agent has to reduce the product stock by Quantity.

        delete curr;    //we are removing previous head after deletion it won't be needed
    }
    else if(n==Tail->Product_ID)
    {
        while(curr!=Tail)
        {
            pre=curr;
            curr=curr->next;
        }

        pre->next=NULL;
        Product_list.setTail(pre);
        Total_Amount-=Tail->Amount;//We have to remove corrsponding Amount from the Total_Amount, as this product is no longer.
        Update_Stock(n,Tail->Quantity);//BILLING agent has to reduce the product stock by Quantity.

        delete curr;    //we are removing previous tail after deletion it won't be needed
    }
    else
    {
        while(curr!=NULL)
        {
            if(curr->Product_ID==n)
            {
                pre->next=curr->next;
                Total_Amount-=curr->Amount;//We have to remove corrsponding Amount from the Total_Amount, as this product is no longer.
                Update_Stock(n,curr->Quantity);//BILLING agent has to reduce the product stock by Quantity.

                delete curr;
                break;
            }
            else
                pre=curr;

            curr=curr->next;
        }
    }
}

inline void BILLING::Assign_Customer_ID(long long n)
{
    Customer_ID=n;
}

void BILLING::Make_Payment()
{
    Transaction_ID=rand();//Transaction_ID for the payment.

    cout<<"TRANSACTION ID :\t"<<setw(10)<<Transaction_ID<<"\n";
    cout<<"CUSTOMER ID    :   \t"<<Customer_ID<<"\n\n";
    cout<<"  PRODUCT ID\t NAME\t\t RATE\t QUANTITY\t AMOUNT\n";

    BILL *ptr=Product_list.getHead();
    while(ptr!=NULL)
    {
        cout<<ptr->Product_ID<<"\t";
        cout<<setw(5)<<ptr->Name<<"\t";
        cout<<setw(13)<<fixed<<setprecision(2)<<ptr->Rate<<"\t";
        cout<<setw(9)<<ptr->Quantity<<"\t";
        cout<<setw(7)<<ptr->Amount<<"\n";
        ptr=ptr->next;
    }
    //Updating points
    Update_Points(Customer_ID,Total_Amount);

    line();
    cout<<"\n"<<setw(50)<<"TOTAL : "<<setw(13)<<fixed<<setprecision(2)<<Total_Amount<<"\n";
    line();
    cout<<"\n\n\n";

    //Data members have to be reinitilized, for the next shopping episode.
    Restore_BILLING_Data();
}

void BILLING::Restore_BILLING_Data()
{
    Total_Amount=0;

    Product_list.setHead(NULL);
    Product_list.setTail(NULL);
}


//////////////////// DRIVER CODE ////////////////////
int main()
{
    //NOTE : I have renamed INVENTORY_DATACSET to Product_database in the CLASS INVENTORY.
    long long INVENTORY_DATASET[100][4] = {
{111100000001,1,100,20},{111100000002,2,110,20},{111100000003,3,120,20},{111100000004,4,130,20},{111100000005,5,140,20},{111100000006,6,150,20},{111100000007,7,160,20},{111100000008,8,170,20},{111100000009,9,180,20},{111100000010,10,190,20},
{111100000011,11,200,20},{111100000012,12,210,20},{111100000013,13,220,20},{111100000014,14,230,20},{111100000015,15,240,20},{111100000016,16,250,20},{111100000017,17,260,20},{111100000018,18,270,20},{111100000019,19,280,20},{111100000020,20,290,20},
{111100000021,21,300,20},{111100000022,22,310,20},{111100000023,23,320,20},{111100000024,24,330,20},{111100000025,25,340,20},{111100000026,26,350,20},{111100000027,27,360,20},{111100000028,28,370,20},{111100000029,29,380,20},{111100000030,30,390,20},
{111100000031,31,400,20},{111100000032,32,410,20},{111100000033,33,420,20},{111100000034,34,430,20},{111100000035,35,440,20},{111100000036,36,450,20},{111100000037,37,460,20},{111100000038,38,470,20},{111100000039,39,480,20},{111100000040,40,490,20},
{111100000041,41,500,20},{111100000042,42,510,20},{111100000043,43,520,20},{111100000044,44,530,20},{111100000045,45,540,20},{111100000046,46,550,20},{111100000047,47,560,20},{111100000048,48,570,20},{111100000049,49,580,20},{222200001111,50,590,20},
{222200001114,51,600,20},{222200001117,52,610,20},{222200001120,53,620,20},{222200001123,54,630,20},{222200001126,55,640,20},{222200001129,56,650,20},{222200001132,57,660,20},{222200001135,58,670,20},{222200001138,59,680,20},{222200001141,60,690,20},
{222200001144,61,700,20},{222200001147,62,710,20},{222200001150,63,720,20},{222200001153,64,730,20},{222200001156,65,740,20},{222200001159,66,750,20},{222200001162,67,760,20},{222200001165,68,770,20},{222200001168,69,780,20},{222200001171,70,790,20},
{222200001174,71,800,20},{222200001177,72,810,20},{222200001180,73,820,20},{222200001183,74,830,20},{222200001186,75,840,20},{222200001189,76,850,20},{222200001192,77,860,20},{222200001195,78,870,20},{222200001198,79,880,20},{222200001201,80,890,20},
{222200001204,81,900,20},{222200001207,82,910,20},{222200001210,83,920,20},{222200001213,84,930,20},{222200001216,85,940,20},{222200001219,86,950,20},{222200001222,87,960,20},{222200001225,88,970,20},{222200001228,89,980,20},{222200001231,90,990,20},
{222200001234,91,1000,20},{222200001237,92,1010,20},{222200001240,93,1020,20},{222200001243,94,1030,20},{222200001246,95,1040,20},{222200001249,96,1050,20},{222200001252,97,1060,20},{222200001255,98,1070,20},{222200001258,99,1080,20},{222200001261,100,1090,20},
};

    //NOTE : I have renamed CUSTOMER_DATASET to Customer_database in the CLASS CUSTOMER_CENTER.
    long long CUSTOMER_DATASET[100][3] = {
{9400000001,1,0},{9400000002,2,0},{9400000003,3,0},{9400000004,4,0},{9400000005,5,0},{9400000006,6,0},{9400000007,7,0},{9400000008,8,0},{9400000009,9,0},{9400000010,10,0},
{9400000011,11,0},{9400000012,12,0},{9400000013,13,0},{9400000014,14,0},{9400000015,15,0},{9400000016,16,0},{9400000017,17,0},{9400000018,18,0},{9400000019,19,0},{9400000020,20,0},
{9400000021,21,0},{9400000022,22,0},{9400000023,23,0},{9400000024,24,0},{9400000025,25,0},{9400000026,26,0},{9400000027,27,0},{9400000028,28,0},{9400000029,29,0},{9400000030,30,0},
{9400000031,31,0},{9400000032,32,0},{9400000033,33,0},{9400000034,34,0},{9400000035,35,0},{9400000036,36,0},{9400000037,37,0},{9400000038,38,0},{9400000039,39,0},{9400000040,40,0},
{9400000041,41,0},{9400000042,42,0},{9400000043,43,0},{9400000044,44,0},{9400000045,45,0},{9400000046,46,0},{9400000047,47,0},{9400000048,48,0},{9400000049,49,0},{9400000050,50,0},
{9400000051,51,0},{9400000052,52,0},{9400000053,53,0},{9400000054,54,0},{9400000055,55,0},{9400000056,56,0},{9400000057,57,0},{9400000058,58,0},{9400000059,59,0},{9400000060,60,0},
{9400000061,61,0},{9400000062,62,0},{9400000063,63,0},{9400000064,64,0},{9400000065,65,0},{9400000066,66,0},{9400000067,67,0},{9400000068,68,0},{9400000069,69,0},{9400000070,70,0},
{9400000071,71,0},{9400000072,72,0},{9400000073,73,0},{9400000074,74,0},{9400000075,75,0},{9400000076,76,0},{9400000077,77,0},{9400000078,78,0},{9400000079,79,0},{9400000080,80,0},
{9400000081,81,0},{9400000082,82,0},{9400000083,83,0},{9400000084,84,0},{9400000085,85,0},{9400000086,86,0},{9400000087,87,0},{9400000088,88,0},{9400000089,89,0},{9400000090,90,0},
{9400000091,91,0},{9400000092,92,0},{9400000093,93,0},{9400000094,94,0},{9400000095,95,0},{9400000096,96,0},{9400000097,97,0},{9400000098,98,0},{9400000099,99,0},{9400000100,100,0},
};

    MANAGER M(111,"Raj");
    //M.Display_Manager_Data();
    for(int i=0;i<100;++i)
        M.AddProduct(INVENTORY_DATASET[i][0],M.num_to_str(INVENTORY_DATASET[i][1]),INVENTORY_DATASET[i][2],INVENTORY_DATASET[i][3]);
    //M.Display_Product_database();

    cout<<endl<<endl<<endl;

    BILLING Agent;
    for(int i=0;i<100;++i)
        Agent.Add_Customer(CUSTOMER_DATASET[i][0],Agent.num_to_str(CUSTOMER_DATASET[i][1]),CUSTOMER_DATASET[i][2]);
    //Agent.Display_Customer_database();



    //One given Shopping Episode
    Agent.Assign_Customer_ID(9400000011);
    Agent.Buy_Product(111100000011, 3);
    Agent.Buy_Product(222200001114, 1);
    Agent.Buy_Product(222200001234, 2);
    //Checking: If customer wants to remove a product from the its productlist then our system works or not.
    //Agent.Remove_Product(111100000011);
    Agent.Make_Payment();

    //An extra Shopping Episode
    /*
    Agent.Assign_Customer_ID(9400000012);
    Agent.Buy_Product(111100000011, 3);
    Agent.Buy_Product(222200001114, 1);
    Agent.Buy_Product(222200001234, 2);
    Agent.Make_Payment();
    */

    cout<<"\n(*Upadated Product Database after one shopping episode is as follows)\n";
    cout<<"  PRODUCT ID\t NAME\t\t RATE\t QUANTITY\n";
    M.DisplayProductData(111100000011);
    M.DisplayProductData(222200001114);
    M.DisplayProductData(222200001234);

    return 0;
}