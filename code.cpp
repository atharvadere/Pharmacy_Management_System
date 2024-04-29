#include<iostream>
#include<string.h>
#include<fstream>
#include <ctime>
#include<graphics.h>
#include<limits>

using namespace std;

class node
{   public:
    node * prev;
    string name;
    int block;
    float price;
    int quantity;
    int expiry;
    string alternative;
    int discount;
    node * next;
};

class skipnode
{   public:
    node *pointer;
    skipnode *next;
};

class cart
{   public:
    node *pointer;
    int quantity;
    cart *next;
};

//Invalid input handler
int ip()
{   
    int x;
    cin >> x;
    while(cin.fail()) 
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout << "\tBad entry!\n\tRenter valid data : ";
        cin >> x;
    }
    return x;
}
float ipf()
{   float x;
    cin >> x;
    while(cin.fail()) 
    {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        cout << "\n\tBad entry!\n\tEnter valid choice : ";
        cin >> x;
    }
    return x;
}

//Insert medicinal data
node * insertuser(node * adr)
{  
    node *p;
    p=new node;
    p->discount=0;
    cout<<"\n\t\tEnter Name :";
    cin>>p->name;
    cout<<"\n\t\tEnter Block :";
    p->block=ip();
    cout<<"\n\t\tEnter Price :";
    p->price=ipf();
    cout<<"\n\t\tEnter Quantity :";
    p->quantity=ip();
    int fg=1;
    while(fg==1)
    {
        cout<<"\n\t\tEnter Expiry [mmyy] :";
        int tmpry;
        tmpry=ip();
        if(tmpry/100>12 || tmpry/100<1)
            cout<<"\n\tEnter valid month !\n";
        else
        {
            p->expiry=tmpry;
            fg=0;
        }
    }
    cout<<"\n\t\tEnter Alternative :";
    cin>>p->alternative;

    if(adr==NULL)
    {
        adr=p;
        p->next=NULL;
        p->prev=NULL;
    }
    else
    {   node* tmp=adr;
        node* t=NULL;
        while(tmp->name.compare(p->name)<=0)
        {
            t=tmp;
            tmp=tmp->next;
            if(tmp==NULL)
            break;
        }
        if(t==NULL)
        {   
            adr->prev=p;
            p->next=adr;
            p->prev=NULL;
            adr=p;
        }
        else if(tmp==NULL)
        {
            t->next=p;
            p->prev=t;
            p->next=NULL;
        }
        else
        {   
            (t->next)->prev=p;
            p->next=t->next;
            p->prev=t;
            t->next=p;
        }
        
    }
    return adr;

}

//Calculate discount
skipnode* discount(node *adr)
{
    time_t now = time(0);   
    tm *ltm = localtime(&now);
    // print various components of tm structure.
    int curryr=(1900 + ltm->tm_year)%100;
    int currmon=1 + ltm->tm_mon;
    skipnode* head=NULL;
    skipnode* tmp=NULL;
    while(adr!=NULL)
    {
        int expmon=(adr->expiry)/100;
        int expyr=(adr->expiry)%100;

        if(expyr>curryr)
            adr->discount=0;
        else if(expyr==curryr)
        {
            if(expmon>currmon)
                adr->discount=0;
            else if(expmon==currmon)
                adr->discount=15;
            else
                {
                    adr->discount=-1;
                }

        }
        else
            {
                adr->discount=-1;

            }
        if(adr->discount==-1)
        {
            skipnode *p=new skipnode;
            p->pointer=adr;
            p->next=NULL;
            if(head==NULL)
            {
                head=p;
            }
            else
            {
                tmp->next=p;
            }
            tmp=p;
        }
        adr=adr->next;
    }
    return head;
}

//Searching using skiplist algorithm
node* search(string key,skipnode * ref)
{   int flag=0;
    while(ref!=NULL)
    {   
        if((ref->pointer)->name.at(0)==key.at(0))
        break;

        ref=ref->next;
    }
    if(ref==NULL)
    return 0;

    node *tmp=ref->pointer;
    while(tmp!=NULL && tmp->name.at(0)==(ref->pointer)->name.at(0))
    {
        if(tmp->name.compare(key)==0)
        {
            return tmp;
        }
        tmp=tmp->next;

    }
    return 0;
}

//Delete medicinal data
node* deletedata(node *linkh,skipnode *skiph)
{
    cout<<"\n\tEnter the item name you want to delete :";
    string tmp;
    cin>>tmp;
    int flagg=0;
    node *temp=search(tmp,skiph);
    if(temp==NULL)
    {
        cout<<"\n\tSorry,Item is absent in the list !\n";
    }
    else if(linkh==temp)
    {   
        linkh=linkh->next;
        linkh->prev=NULL;
        delete temp;
        flagg=1;
    }
    else if(temp->next==NULL)
    {
        (temp->prev)->next=NULL;
        delete temp;
        flagg=1;
    }
    else
    {
        (temp->prev)->next=temp->next;
        (temp->next)->prev=temp->prev;
        delete temp;
        flagg=1;
    }
    if(flagg==1)
        cout<<"\n\tEntry Successfully Deleted !";
    return linkh;
}


//Store linked list in txt file
void store(node *adr,int flag)
{   
    if(adr==NULL)
    return;

    fstream obj;
    switch(flag)
    {
        case 0:
            obj.open("tablet.txt",ios::out);
            break;
            
            case 1:
            obj.open("syrup.txt",ios::out);
            break;
            
            case 2:
            obj.open("capsule.txt",ios::out);
            break;
            
            case 3:
            obj.open("drop.txt",ios::out);
            break;

            case 4:
            obj.open("inhaler.txt",ios::out);
            break;
            
            case 5:
            obj.open("injection.txt",ios::out);
            break;
    }

    node *p=adr;
    while(p->next!=NULL)
    {obj<<p->name<<"\t"<<p->block<<"\t"<<p->price<<"\t"<<p->quantity<<"\t"<<p->expiry<<"\t"<<p->alternative<<"\n";
    p=p->next;
    }
    obj<<p->name<<"\t"<<p->block<<"\t"<<p->price<<"\t"<<p->quantity<<"\t"<<p->expiry<<"\t"<<p->alternative;
    obj.close();
}

//Create linked list of stored data
node* retrieve(node *adr,int flag)
{
        fstream obj1;
        switch(flag)
        {
            case 0:
            obj1.open("tablet.txt",ios::in);
            break;
            
            case 1:
            obj1.open("syrup.txt",ios::in);
            break;
            
            case 2:
            obj1.open("capsule.txt",ios::in);
            break;
            
            case 3:
            obj1.open("drop.txt",ios::in);
            break;

            case 4:
            obj1.open("inhaler.txt",ios::in);
            break;
            
            case 5:
            obj1.open("injection.txt",ios::in);
            break;
        }
        if(!obj1)
            {   
                obj1.close();
                return NULL;
            }
        node *temp=NULL;
        while(!obj1.eof())
        {    node *s;
            s=new node;
            s->next=NULL;
            s->discount=0;
            if(adr==NULL)
            {
                adr=s;
                temp=s;
                s->prev=NULL;
            }
            else
            {
                temp->next=s;
                s->prev=temp;
                temp=s;
            }

        obj1>>s->name>>s->block>>s->price>>s->quantity>>s->expiry>>s->alternative;
        }
        obj1.close();
        return adr;
}

//Create skiplist
skipnode* skiplist(node *adr)
{   if(adr==NULL)
        return NULL;
    node *temp=adr;
    node *tmp=temp->next;
    skipnode *p=new skipnode;
    p->next=NULL;
    p->pointer=temp;
    skipnode *skiphead=p;
    skipnode *temppointer=p;
    while(tmp!=NULL)
    {   
        if(tmp->name.at(0)!=temp->name.at(0))
        {   skipnode *p=new skipnode;
            p->next=NULL;
            p->pointer=tmp;
            temppointer->next=p;
            temppointer=p;
        }
        tmp=tmp->next;
        temp=temp->next;
    }
    return skiphead;

}

//Display all stored medicines
int display(node* ads,int flag)
{   if(ads==NULL)
        {
            return 0;
        }
    cout<<endl<<endl<<endl;
    for(int i=0;i<12;i++) //8
        cout<<"   ";
    switch(flag)
        {   
            case 0:
            cout<<"---------------------------------Tablets---------------------------------\n";
            break;
            case 1:
            cout<<"---------------------------------Syrups---------------------------------\n";
            break;
            case 2:
            cout<<"---------------------------------Capsules---------------------------------\n";
            break;
            case 3:
            cout<<"---------------------------------Drops---------------------------------\n";
            break;
            case 4:
            cout<<"---------------------------------Inhalers---------------------------------\n";
            break;
            case 5:
            cout<<"---------------------------------Injections---------------------------------\n";
            break;
        }
    node* s=ads;
    cout<<"\n\tName\t\t|\tBlock\t|\tPrice\t|\tQuantity\t|\t";
    cout<<"Expiry\t\t|\tAlternative\t|\tDiscount\n\t";
    for(int i=0;i<137;i++)
    cout<<"-";

    while(s!=NULL)
    {   
        cout<<"\n\t"<<s->name;
        if(s->name.size()>=8)
            cout<<"\t|\t";
        else
            cout<<"\t\t|\t";
        cout<<s->block<<"\t|\t"<<s->price<<"\t|\t"<<s->quantity<<"\t\t|\t";
        cout<<s->expiry/100<<"/20"<<s->expiry%100<<"\t\t|\t"<<s->alternative;
        if(s->alternative.size()>=8)
            cout<<"\t|\t";
        else
            cout<<"\t\t|\t";
        if(s->discount==-1)
            cout<<"expired";
        else
            cout<<s->discount<<"%";
        s=s->next;
    }
    cout<<endl;
    return 1;
}

//Display skiplist
/*void manage :: display(skipnode* ads)
{       if(ads==NULL)
        {
            cout<<"\n\tNothing to display !";
            return;
        }
        skipnode* s=ads;
        cout<<"\n\tPointer\t\t|\tNext\n\t";
        for(int i=0;i<33;i++)
        cout<<"-";
        while(s!=NULL)
        {   
            cout<<"\n\t"<<s->pointer<<"\t|\t"<<s->next;
            s=s->next;
        }
        cout<<endl;
}
*/

//Display cart items
void display(cart* ads)
{
    cout<<"\n\t\tName\t|\tPrice\t|\tQuantity\t|\tExpiry\t|\tDiscount\n\t";
        for(int i=0;i<90;i++)
        cout<<"-";
        node *s=NULL;
        do
        {   
            s=ads->pointer;
            cout<<"\n\t"<<s->name;
            if(s->name.size()>=8)
                cout<<"\t|\t";
            else
                cout<<"\t\t|\t";
            cout<<s->price<<"\t|\t"<<ads->quantity<<"\t\t|\t"<<s->expiry/100<<"/20"<<s->expiry%100<<"\t|\t"<<s->discount<<"%";
            ads=ads->next;
            
        }
        while(ads!=NULL);
}

//Calculate Total bill value
void bill(cart *billadr)
{
    float total=0;
    cart * s=billadr;
    while(s!=NULL)
    {
        node *tmp=s->pointer;
        total=total+(tmp->price)*(s->quantity)*(100-tmp->discount)/100;
        s=s->next;
    }
    display(billadr);
    cout<<"\n\n\tTotal :\t\t"<<total;
    cout<<endl;

}

//Display expired medicines
int display(skipnode * adr,int flg)
{   if(adr==NULL)
        {
            return 0;
        }
    cout<<endl<<endl<<endl;
    for(int i=0;i<3;i++)
        cout<<"\t";
    switch(flg)
        {   
            case 0:
            cout<<"---------------------------------Tablets---------------------------------\n";
            break;
            case 1:
            cout<<"---------------------------------Syrups---------------------------------\n";
            break;
            case 2:
            cout<<"---------------------------------Capsules---------------------------------\n";
            break;
            case 3:
            cout<<"---------------------------------Drops---------------------------------\n";
            break;
            case 4:
            cout<<"---------------------------------Inhalers---------------------------------\n";
            break;
            case 5:
            cout<<"---------------------------------Injections---------------------------------\n";
            break;
        }
    cout<<"\n\tName\t\t|\tBlock\t|\tPrice\t|\tQuantity\t|\t";
    cout<<"Expiry\t|\tAlternative\n\t";
    for(int i=0;i<117;i++)
    cout<<"-";
    while(adr!=NULL)
    {
        node *s=adr->pointer;
        cout<<"\n\t"<<s->name;
        if(s->name.size()>=8)
            cout<<"\t|\t";
        else
            cout<<"\t\t|\t";
        cout<<s->block<<"\t|\t"<<s->price<<"\t|\t"<<s->quantity<<"\t\t|\t";
        cout<<s->expiry/100<<"/20"<<s->expiry%100<<"\t|\t"<<s->alternative;
        adr=adr->next;
    } 
    cout<<endl;
    return 1;
}

//display bar graphs
void bargraph(int arr[],int tsize,bool daymon,bool numworth)
{   float max=0;
    int gd=DETECT,gm;
    initgraph(&gd,&gm,NULL);
    int xm=getmaxx();
    int ym=getmaxy();

    for(int i=0;i<tsize;i++)
    {
        if(arr[i]>max)
            max=arr[i];
    }
    float sc=(ym-100)/max;
    float step=(xm-100)/tsize;
    setcolor(WHITE);
    line(50,ym-50,50,50);
    line(50,ym-50,xm-50,ym-50);
    settextstyle(8,1,1);
    for(int i=0;i<tsize;i++)
    {   setcolor(i%8+7);
        rectangle(50+step/2+i*step,ym-50-sc*arr[i],50+(i+1)*step,ym-50);
        setfillstyle(LTSLASH_FILL,i%8+8);
        floodfill(50+0.75*step+i*step,ym-50-(sc/2)*arr[i],i%8+7);
        if(arr[i]!=0)
            outtextxy(50+(i+1)*step,ym-80-sc*arr[i],&to_string(arr[i])[0]);
        outtextxy(50+(i+1)*step,ym-40,&to_string(i+1)[0]); //convert to string and passing address
    }
    string y,x;
    if(daymon)
        x="Months";
    else
        x="Days";
    if(numworth)
        y="Worth of Medicines sold in Rs";
    else
        y="Number of Medicines sold";
    setcolor(WHITE);
    outtextxy(40,80,&y[0]);
    settextstyle(8,0,1);
    outtextxy(xm/2-7,ym-5,&x[0]);
    getch();
    delay(100);
    closegraph();
    cout<<"\n\tGraph displayed successfully !";
    
}

//Record and display sale stats
void statistics(cart* adr,int flag)
{   fstream obj;
    time_t now = time(0); // get current date and time    
    tm* ltm = localtime(&now);  
    switch(flag)
    {   
        case 0:
        obj.open("statistics.txt",ios::app);
        if(obj)
            obj<<endl;
        while(adr->next!=NULL)
        {   node *temp=adr->pointer;
            obj<<ltm->tm_mday<<"\t"<<1 + ltm->tm_mon<<"\t"<<temp->name<<"\t"<<temp->price;
            obj<<"\t"<<adr->quantity<<"\t"<<temp->discount<<"\t"<<ltm->tm_hour<<":"<<ltm->tm_min <<"\t"<<1900 + ltm->tm_year<<endl;
            adr=adr->next;
        }
        obj<<ltm->tm_mday<<"\t"<<1 + ltm->tm_mon<<"\t"<<adr->pointer->name<<"\t"<<adr->pointer->price;
        obj<<"\t"<<adr->quantity<<"\t"<<adr->pointer->discount<<"\t"<<ltm->tm_hour<<":"<<ltm->tm_min <<"\t"<<1900 + ltm->tm_year;
        break;

        case 1:
        obj.open("statistics.txt",ios::in);
        if(!obj)
        {
            cout<<"\n\tNo purchases are made !";
            obj.close();
            return ;
        }
        int cce;
        cout<<"\n\tSelect the Type :\n\t1.Detailed List\n\t2.Graph\n\tEnter your choice :";
        cce=ip();
        if(cce==1)
        {

            int choice;
            cout<<"\n\tSelect the Duration :\n\t1.Today\n\t2.This Month\n\t3.All\n\tEnter your choice :";
            choice=ip();
            if(choice>3 || choice<1)
                throw "ecpt";

                cout<<"\n\tName\t\t|\tPrice\t|\tQuantity\t|\tDiscount\t|\tTime\t|\tDate\t\t|\tTotal"<<endl<<"\t";
                for(int i=0;i<132;i++)
                    cout<<"-";
                cout<<endl;
                float total=0;
                int qut=0;
                switch(choice)
                {   
                    case 1:
                    while(!obj.eof())
                    {   
                        int todaydate,currmon,quantity,discount,year;
                        float price;
                        string name,time; 
                        obj>>todaydate>>currmon>>name>>price>>quantity>>discount>>time>>year;
                        if(todaydate==ltm->tm_mday && currmon==1 + ltm->tm_mon && year==1900 + ltm->tm_year)
                        {
                            cout<<"\t"<<name;
                            if(name.size()>=8)
                                cout<<"\t|\t";
                            else
                                cout<<"\t\t|\t";
                            cout<<price<<"\t|\t"<<quantity<<"\t\t|\t"<<discount;
                            cout<<"\t\t|\t"<<time<<"\t|\t"<<todaydate<<"/"<<currmon<<"/"<<year<<"\t|\t";
                            float temp=quantity*price*(100-discount)/100;
                            cout<<temp<<endl;
                            total=total+temp;
                            qut=qut+quantity;
                        }
                    }
                    cout<<"\n\tFor Today :-\n\n\tNet Collection : Rs "<<total<<"\n\tMedicines sold : "<<qut<<endl;
                    break;

                    case 2:
                    while(!obj.eof())
                    {   int todaydate,currmon,quantity,discount,year;
                        float price;
                        string name,time; 
                        obj>>todaydate>>currmon>>name>>price>>quantity>>discount>>time>>year;
                        if(currmon==1 + ltm->tm_mon && year==1900 + ltm->tm_year)
                        {
                            cout<<"\t"<<name;
                            if(name.size()>=8)
                                cout<<"\t|\t";
                            else
                                cout<<"\t\t|\t";
                            cout<<price<<"\t|\t"<<quantity<<"\t\t|\t"<<discount;
                            cout<<"\t\t|\t"<<time<<"\t|\t"<<todaydate<<"/"<<currmon<<"/"<<year<<"\t|\t";
                            float temp=quantity*price*(100-discount)/100;
                            cout<<temp<<endl;
                            total=total+temp;
                            qut=qut+quantity;
                        }
                    }
                    
                    cout<<"\n\tThis Month :-\n\n\tNet Collection : Rs "<<total<<"\n\tMedicines sold : "<<qut<<endl;
                    break;

                    case 3:
                    while(!obj.eof())
                    {   int todaydate,currmon,quantity,discount,year;
                        float price;
                        string name,time; 
                        obj>>todaydate>>currmon>>name>>price>>quantity>>discount>>time>>year;
                        cout<<"\t"<<name;
                            if(name.size()>=8)
                                cout<<"\t|\t";
                            else
                                cout<<"\t\t|\t";
                            cout<<price<<"\t|\t"<<quantity<<"\t\t|\t"<<discount;
                        cout<<"\t\t|\t"<<time<<"\t|\t"<<todaydate<<"/"<<currmon<<"/"<<year<<"\t|\t";
                        float temp=quantity*price*(100-discount)/100;
                        cout<<temp<<endl;
                        total=total+temp;
                        qut=qut+quantity;
                    }
                    cout<<"\n\tOverall :-\n\n\tNet Collection : Rs "<<total<<"\n\tMedicines sold : "<<qut<<endl;
                    break;

                }
        
        }

        else if(cce==2)
        {   
            int choice;
            cout<<"\n\tSelect the Duration for sale statistics :\n\t1.This Month\n\t2.This year\n\tEnter your choice :";
            choice=ip();
            if(!(choice==1 || choice==2))
            {
                obj.close();
                throw "ecpt";
            }
            short fg;
            cout<<"\n\tSelect the Criteria :\n\t1.Quantity\n\t2.Value\n\tEnter your choice :";
            fg=ip();
            if(!(fg==1 || fg==2))
            {
                obj.close();
                throw "ecpt";
            }
            if(choice==1)
            {   int cnt[31];
                for(int i=0;i<31;i++)
                    cnt[i]=0;
                while(!obj.eof())
                    {   int todaydate,currmon,quantity,discount,year;
                        float price;
                        string name,time; 
                        obj>>todaydate>>currmon>>name>>price>>quantity>>discount>>time>>year;
                        if(currmon==1 + ltm->tm_mon && year==1900 + ltm->tm_year)
                        {   
                            if(fg==1)
                                cnt[todaydate-1]++;
                            else
                                cnt[todaydate-1]=cnt[todaydate-1] + quantity*price*(100-discount)/100;
                        }
                    }
                
                if(fg==1)
                    bargraph(cnt,31,0,0);
                else
                    bargraph(cnt,31,0,1);
            }
            else if(choice==2)
            {
                int cnt[12];
                for(int i=0;i<12;i++)
                    cnt[i]=0;
                while(!obj.eof())
                    {   int todaydate,currmon,quantity,discount,year;
                        float price;
                        string name,time; 
                        obj>>todaydate>>currmon>>name>>price>>quantity>>discount>>time>>year;
                        if(year==1900 + ltm->tm_year)
                        {
                            if(fg==1)
                                cnt[currmon-1]++;
                            else
                                cnt[currmon-1]=cnt[currmon-1] + quantity*price*(100-discount)/100;
                        }
                    } 
                if(fg==1)
                    bargraph(cnt,12,1,0);
                else
                    bargraph(cnt,12,1,1);
            }
        }

        else
            throw "ecpt";
    }
    obj.close();
}

//compare two char arrays
bool compare(char arr1[8],char arr2[8])
{   bool flag=1;
    for(int i=0;i<8;i++)
    {   
        if(arr1[i]!=arr2[i])
        {
            flag=0;
            break;
        }
    }
    return flag;
}

//verify login credentials
short loginverify()
{
    cout<<"\n\n\t****/WELCOME/****";
    cout<<"\n\t1.Customer [Limited access]\n\t2.Seller [Full access,Login Required]\n\tSelect the login type :";
    short ltype;
    ltype=ip();
    if(ltype==2)
    {
        fstream obj;
        obj.open("login.txt",ios::in | ios::binary);
        if(!obj)
        {   
            obj.close();
            cout<<"\n\tCreate Seller account to proceed :";
            obj.open("login.txt",ios::out | ios::binary);
            char newuser[8],newpass[8];
            block:
            cout<<"\n\tEnter new username {Length-7} :";
            cin>>newuser;
            cout<<"\tEnter new password {Length-8} :";
            cin>>newpass;
            obj.write(newuser,sizeof(newuser));
            obj.write(newpass,sizeof(newpass));
            obj.close();
            cout<<"\n\tUsername and password created successfully !";
        }
        else
            obj.close();
        char vuser[8],vpass[8],user[8],pass[8];
        obj.open("login.txt",ios::in | ios::binary);
        obj.read(vuser,sizeof(vuser));
        obj.read(vpass,sizeof(vpass));
        obj.close();
        for(int i=0;i<3;i++)
        {   
            cout<<"\n\tEnter Username :";
            cin>>user;
            cout<<"\tEnter Password :";
            cin>>pass;
            
            if(compare(vuser,user)==1 && compare(vpass,pass)==1)
            {
                cout<<"\n\tYou are logged in successfully !";
                return 2;
            }  
            else
            {
                cout<<"\n\tPlease enter valid Username or Password !";
            }  
            if(i==2)
            {
                cout<<"\n\tSorry,You have exceeded the limit !";
                cout<<"\n\t1.Create New login credentials\n\t2.Exit\n\tEnter your choice :";
                short choice;
                choice=ip();

                if(choice==1)
                {   cout<<"\n\tWarning ! All stored data will be deleted !\n\tEnter 'Y' to confirm or 'N' to Exit :";
                    char c;
                    cin>>c;
                    if(c=='Y')
                    {
                        remove("login.txt");
                        remove("tablet.txt");
                        remove("injection.txt");
                        remove("drop.txt");
                        remove("syrup.txt");
                        remove("inhaler.txt");
                        remove("capsule.txt");
                        remove("statistics.txt");
                        cout<<"\n\tPreviously stored data deleted successfully !\n\tYou can now proceed to create new login credentials !\n";
                        goto block;
                        return loginverify();
                    }
                    else
                        choice=2;
                    
                }
                if(choice==2)
                    return 0;
            }
        }
        
    }
    else if(ltype==1)
        return 1;
    else
        {
            cout<<"\n\tPlease enter valid choice !";
            return loginverify();
        }
}

//Free memory before exiting
void freemem(node *adr)
{
    while(adr!=NULL)
    {   
        node *tmp=adr;
        adr=adr->next;
        delete tmp;
    }
}
void freemem(skipnode *adr)
{
    while(adr!=NULL)
    {   
        skipnode *tmp=adr;
        adr=adr->next;
        delete tmp;
    }
}
void freemem(cart *adr)
{
    while(adr!=NULL)
    {   
        cart *tmp=adr;
        adr=adr->next;
        delete tmp;
    }
}


//Main Function
int main()
{   
    short logverify=loginverify();
    if(logverify==0)
        return 0;
    //short logverify=2;
    node *head[6];
    skipnode *skiphead[6];
    skipnode* expadr[6];
    for(int i=0;i<6;i++)
    {
        head[i]=NULL;
        skiphead[i]=NULL;
        expadr[i]=NULL;
        head[i]=retrieve(head[i],i);
        if(head[i]!=NULL)   
        {
            skiphead[i]=skiplist(head[i]);
            //medicine[i].display(skiphead[i]);
            expadr[i]=discount(head[i]);
        }
    }

    while(1)
    {   
        cout<<"\n\n\t******WELCOME******";
        cout<<"\n\t1.Search\n\t2.Bill\n\t3.Exit";
        if(logverify==2)
            cout<<"\n\t4.Insert data\n\t5.Delete data\n\t6.Renew Stock\n\t7.Statistics\n\t8.Manage Expired medicines";
        cout<<"\n\tEnter your choice :";
        int ch;
        ch=ip();
        //store,discoumt,skiplist flag
        int fg=0;
        switch (ch)
        {
            int choice;
            int count;

        case 1:
        {   
            cout<<"\n\tSelect the section from which you want to Search data\n\t1.Tablet\t2.Syrup\n\t3.capsule\t4.Drop\n\t5.Inhaler\t6.Injection";
            cout<<"\n\tEnter your choice :";
            choice=ip();
            if(choice>6 || choice<1)
            {
                cout<<"\n\tPlease enter valid choice !";
                break;
            }
            int dfg;
            dfg=display(head[choice-1],choice-1);
            if(!dfg)
            {
                cout<<"\n\tDatabase not found !\n\tEither insert or import data !";
                break;
            }
            cout<<"\n\tEnter the element you want to search :";
            string k;
            cin>>k;
            node *temp=search(k,skiphead[choice-1]);
            if(temp==NULL)
            {
                cout<<"\n\tSorry,medicine not found !";
            }
            else
            {
                cout<<"\n\tMedicine found :";
                cout<<"\n\tName :"<<temp->name<<"\tPrice :"<<temp->price<<"\tQuantity :"<<temp->quantity<<"\tExpiry :"<<temp->expiry/100<<"/20"<<temp->expiry%100;
            }
            break;
        }

        case 2:
        {   //Display
            int dfg=0;
            for(int i=0;i<6;i++)
            {
                dfg=dfg+display(head[i],i);
            }    
            cout<<endl;
            if(dfg==0)
            {
                cout<<"\n\tDatabase not found for all categories of medicines\n\tEither insert or import data to proceed !\n";
                break;
            }
            else if(dfg<6)
            {
                cout<<"\n\tDatabase not found for some categories of medicines\n\tEither insert or import data !";
                cout<<"\n\tStill you can proceed with other categories !";
            }    

            int ch1;
            cart *carthead=NULL;
            cart *ptr=NULL;
            do
            {
            cout<<"\n\n\t******WELCOME******";
            cout<<"\n\tEnter item to add in cart :";
            string tmp;
            cin>>tmp;
            node *addr=NULL;
            node *addrarr[6];
            short cnts=0;
            bool fg1=0;
            for(int j=0;j<6;j++)
            { 
                addrarr[j]=search(tmp,skiphead[j]);
                if(addrarr[j]!=0)
                {
                    cnts++;
                    if(fg1==0)
                    {
                        addr=addrarr[j];
                        fg1=1;
                    }
                }       
            }
            if(cnts>1)
            {
                cout<<"\n\tSelected medicine is available in "<<cnts<<" sections :";
                for(int i=0;i<6;i++)
                {
                    if(addrarr[i]!=0)
                    {
                        switch(i+1)
                        {
                            case 1:
                            cout<<"\n\t1.Tablet";
                            break;
                            case 2:
                            cout<<"\n\t2.Syrup";
                            break;
                            case 3:
                            cout<<"\n\t3.Capsule";
                            break;
                            case 4:
                            cout<<"\n\t4.Drop";
                            break;
                            case 5:
                            cout<<"\n\t5.Inhaler";
                            break;
                            case 6:
                            cout<<"\n\t6.Injection";
                            break;
                            
                        }
                        
                    }
                }
                cout<<"\n\tEnter your choice :";
                int ch1;
                ch1=ip();
                addr=addrarr[ch1-1];
            }
            
            if(addr==0)
            {
                cout<<"\n\tSorry,the item is out of stock !\n";
            }

            else if(addr->discount==-1)
            {
                cout<<"\n\tSorry,The medicine has expired !";
                if(addr->alternative!="none")
                {
                    cout<<"\n\tFollowing alternative medicine is available :";
                    cout<<addr->alternative<<endl;
                }
            }
            else
            {
                cout<<"\n\tHow many items to add in cart ? (Enter 0 to cancel the item) :";
                int ch;
                ch=ip();
                if(ch>addr->quantity)
                {
                    cout<<"\n\tSorry,only "<<addr->quantity<<" medicines are available in stock !";
                    if(addr->alternative!="-1")
                    {
                        cout<<"\n\tFollowing alternative medicine is available :";
                        cout<<addr->alternative<<endl;
                    }
                }
                else if(ch>0)
                {   
                    cart *p=new cart;
                    p->next=NULL;
                    p->pointer=addr;
                    p->quantity=ch;
                    addr->quantity=addr->quantity-ch;
                    if(carthead==NULL)
                    {
                        carthead=p;
                        ptr=p;
                    }
                    else
                    {
                        ptr->next=p;
                        ptr=p;
                    }
                    cout<<"\t"<<ch<<" "<<addr->name<<"'s successfully added to cart !\n";
                }
            }
            cout<<"\n\tWhat do you want \n\t1.Add more items\n\t2.Proceed for billing\n\t3.Exit";
            cout<<"\n\tEnter your choice :";
            ch1=ip();
            if(ch1==2)
            {   
                if(carthead==NULL)
                    cout<<"\n\tSorry, Cart is empty !";
                else
                    {
                        statistics(carthead,0);
                        bill(carthead);
                        fg=1;
                        freemem(carthead);
                        if(logverify==1)
                            cout<<"\n\tThank You !\n\tVisit Again !";
                    }
            }
            }
            while(ch1==1);
            break;
        }

        case 3:
        for(int i=0;i<6;i++)
        {
            store(head[i],i);
            freemem(head[i]);
            freemem(skiphead[i]);
            head[i]=NULL;
            skiphead[i]=NULL;
        } 
        cout<<"\n\tThank You !";
        delay(1500);
        return 0;

        case 4: 
        if(logverify==2)
        { 
            cout<<"\n\tSelect the section to Insert data\n\t1.Tablet\t2.Syrup\n\t3.capsule\t4.Drop\n\t5.Inhaler\t6.Injection";
            cout<<"\n\tEnter your choice :";
            choice=ip();
            if(!(choice<7 && choice>0))
                {
                    cout<<"\n\tPlease Enter Valid Choice !";
                    break;
                }
            cout<<"\n\tEnter No. of Entries :";
            count=ip();
            for(int i=0;i<count;i++)
            {
                cout<<"\n\tEntry No "<<i+1<<" :";
                head[choice-1]=insertuser(head[choice-1]);  
            }
            cout<<"\n\tData inserted successfully !";
            fg=4;
            break;
        }
        else
        {
            cout<<"\n\tEnter valid choice !"; 
            break;
        }

        case 5:
        if(logverify==2)
        {
            cout<<"\n\tSelect the section from which you want to delete data\n\t1.Tablet\t2.Syrup\n\t3.capsule\t4.Drop\n\t5.Inhaler\t6.Injection";
            cout<<"\n\tEnter your choice :";
            choice=ip();
            if(!(choice<7 && choice>0))
                {
                    cout<<"\n\tPlease Enter Valid Choice !\n";
                    break;
                }
            int dfg;
            dfg=display(head[choice-1],choice-1);
            if(!dfg)
            {
                cout<<"\n\tDatabase not found !\n\tEither insert or import data !";
                break;
            }
            cout<<"\n\tEnter No. of Entries :";
            count=ip();
            for(int i=0;i<count;i++)
                head[choice-1]=deletedata(head[choice-1],skiphead[choice-1]);
            fg=2;  
            break;
        }
        else
        {
            cout<<"\n\tEnter valid choice !";
            break;
        }   

        case 7:
        if(logverify==2)
        {
            //Statistics
            try
            {
                statistics(NULL,1);
            }
            catch(...){
                cout<<"\n\tPlease Enter Valid Choice !";
            }
            break;
        }
        else
        {
            cout<<"\n\tEnter valid choice !";
            break;
        } 

        case 8:
        if(logverify==2)
        {    //Expired medicines
            int defg=0;  //display expired flag
            for(int i=0;i<6;i++)
            {
                defg=defg+display(expadr[i],i);
            }
            if(defg==0)
            {
                cout<<"\n\tNo expired medicines found !";
                break;
            }
            char c;
            cout<<"\n\tDo you want to renew stock ? (y/n)\n\tEnter your choice :";
            cin>>c;
            if(c=='n'||c=='N')
            break;
        }
        else
        {
            cout<<"\n\tEnter valid choice !";
            break;
        } 

        case 6:
        if(logverify==2)
        {   
            int flg;
            cout<<"\n\tSelect the section for which you want to Renew stock\n\t1.Tablet\t2.Syrup\n\t3.capsule\t4.Drop\n\t5.Inhaler\t6.Injection";
            cout<<"\n\tEnter your choice :";
            choice=ip();
            if(choice>6 || choice<1)
            {
                cout<<"\n\tPlease enter valid choice !\n";
                break;
            }
            if(head[choice-1]==NULL)
            {
                cout<<"\n\tDatabase not found !\n\tEither insert or import data !";
                break;
            }
            cout<<"\n\t1.Bulk Update\n\t2.Single\n\tEnter your choice :";
            flg=ip();
            if(flg==1)
            {
                node * tmp=head[choice-1];
                while(tmp!=NULL)
                {   int che;
                    cout<<"\n\tName :"<<tmp->name<<"\tPrice :"<<tmp->price<<"\tQuantity :"<<tmp->quantity<<"\tExpiry :"<<tmp->expiry/100<<"/20"<<tmp->expiry%100;
                    cout<<"\n\tDo you want to update stock ?\n\t1.Yes\n\t2.No\n\t3.Exit\n\tEnter your choice :";
                    che=ip();
                    if(che==1)
                    {   int tmpr;
                        cout<<"\n\tEnter Updated Data :\n\tQuantity :";
                        tmpr=ip();
                        tmp->quantity=tmpr+tmp->quantity;
                        cout<<"\n\tExpiry (mmyy):";
                        tmp->expiry=ip();
                        cout<<"\n\tData updated successfully !";
                        fg=3;
                    }
                    else if(che==3)
                    {
                        break;
                        break;
                    }
                    tmp=tmp->next;
                }
                if(tmp==NULL)
                cout<<"\n\t You have reached end of list !";
            }
            else
            {
                display(head[choice-1],choice-1);
                while(1)
                {
                    string key;
                    cout<<"\n\n\tEnter name :";
                    cin>>key;
                    node* ptr=search(key,skiphead[choice-1]);
                    if(ptr==0)
                    {   char fg5;
                        cout<<"\n\tMedicine not found";
                        cout<<"\n\tDo you want to continue ? (y/n) :";
                        cin>>fg5;
                        if(fg5=='n' ||fg5=='N')
                        {
                            break;
                           
                        }
                    }
                    else
                    {
                        int che;
                        cout<<"\n\tName :"<<ptr->name<<"\tPrice :"<<ptr->price<<"\tQuantity :"<<ptr->quantity<<"\tExpiry :"<<ptr->expiry;
                        cout<<"\n\tDo you want to update stock ?\n\t1.Yes\n\t2.No\n\tEnter your choice :";
                        che=ip();
                        if(che==1)
                        {
                            cout<<"\n\tEnter Updated Data :\n\tQuantity :";
                            int tmpq;
                            tmpq=ip();
                            ptr->quantity=ptr->quantity+tmpq;
                            cout<<"\tExpiry [mmyy] :";
                            ptr->expiry=ip();
                            cout<<"\n\tData updated successfully !";
                            fg=3;
                            char fg3;
                            cout<<"\n\tDo you want to continue ? (y/n) :";
                            cin>>fg3;
                            if(fg3=='n' ||fg3=='N')
                            {
                                break;
                            }
                        }
                    }
                }
            }
            
            break;
        }
        else
        {
            cout<<"\n\tEnter valid choice !";
            break;
        } 
        
        default:
            cout<<"\n\tPlease enter valid choice !";
    }

        if(fg!=0)
        {
            for(int i=0;i<6;i++)
            {   
                store(head[i],i);    
                if(fg==2)
                    skiphead[i]=skiplist(head[i]);
                else if(fg==3)
                    expadr[i]=discount(head[i]);
                else if(fg==4)
                    {
                        skiphead[i]=skiplist(head[i]);
                        expadr[i]=discount(head[i]);
                    }        
            }
        }  
    }
}