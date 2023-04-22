#include<iostream>
using namespace std;
class Node{
	public:
	int size;
	int fragment;
	int a;
	Node* prev;
	Node* next;
	Node(int size,int fragment)
	{
		this->size=size;
		this->fragment=fragment;
		this->a=0;
		this->next=NULL;
	}
};
Node* head;
void HeapSize(int heap)
{
	if(heap%8<4)
	heap-=heap%8;
	else
	heap+=8-heap%8;
	head=new Node(heap,0);
	cout<<"Heap Size="<<head->size<<endl;
}
Node* allocate(Node* head,int size)
{
	int fr=0,size1=size;
	if(size%8!=0)
	{
	    size1=(size/8 +1)*8;
	    fr=8-size%8;
	}
	Node* head1=head;
	Node* newNode=new Node(size,fr);
	newNode->a=1;
	if(head1->next==NULL)
	{
		newNode->next=head1;
		head1->size=head1->size-size1;
		return newNode;
	}
	while(head1->next->next!=NULL)
	head1=head1->next;
	Node* remain=head1->next;
	newNode->prev=head1;
	head1->next=newNode;
	newNode->next=remain;
	remain->prev=newNode;
	remain->size=remain->size-size1;
	if(remain->size<=0)
	{
	    newNode->next=NULL;
		delete(remain);	
	}
	return head;
}
int remaining_memory(Node* head)
{
	Node* head1=head;
	while(head1->next!=NULL)
	head1=head1->next;
	return head1->size;
}
void Wanttoallocate()
{
	int a;
	Node* head1;
	do{
		int size;
		cout<<"Enter size to allocate in memory";
		cin>>size;
		if(size<=0)
		continue;
		int size1=size;
		if(size%8!=0)
		size1=size+8-size%8;
		if(size1==remaining_memory(head))
		{
			head=allocate(head,size);
		    cout<<"Remaining memory="<<0<<endl;
		    cout<<"You can't allocate further!"<<endl;
		    break;
		}
		if(remaining_memory(head)<size)
		cout<<"Unable to Allocate only "<<remaining_memory(head)<<" is available."<<endl;
		else
		head=allocate(head,size);
		head1=head;
	    while(head1!=NULL)
	    {
		    cout<<"| "<<head1->size<<" "<<head1->fragment<<" "<<head1->a<<" |";
		    head1=head1->next;
	    }
	    cout<<endl;
		cout<<"Remaining memory="<<remaining_memory(head)<<endl;
		cout<<"Press any key to allocate again else press 0 ";
		cin>>a;
	}while(a!=0);
	head1=head;
	while(head1!=NULL)
    {
	    cout<<"| "<<head1->size<<" "<<head1->fragment<<" "<<head1->a<<" |";
	    head1=head1->next;
    }
    cout<<endl;
}
void merge(Node* head)
{
	Node* head1=head;
	while(head1!=NULL)
	{
		if(head1->next==NULL)
		return;
		else if(head1->next->next==NULL)
		{
			if(head1->a==0&&head1->next->a==0)
	        {
		        head1->size=head1->size+head1->next->size+head1->fragment+head1->next->fragment;
		        head1->next=NULL;
		        head1->fragment=0;
		        break;
	        }
	        else if(head1->a==0)
	        {
			    head1->size=head1->size+head1->fragment;
			    head1->fragment=0;
			    break;
			}
	        head1=head1->next;
		}
		else 
		{
			if(head1->a==0&&head1->next->a==0)
	        {
		        Node* newNode=head1->next->next;
		        head1->size=head1->size+head1->next->size+head1->fragment+head1->next->fragment;
		        head1->next=newNode;
		        head1->fragment=0;
		        merge(head1);
	        }
	        else if(head1->a==0)
	        {
			    head1->size=head1->size+head1->fragment;
			    head1->fragment=0;
			}
	        head1=head1->next;
		}
	}
}
Node* free(Node* head,int size)
{
	Node* head1=head;
	int x=0;
	while(head1!=NULL)
	{
		if(head1->size==size&&head1->a==1)
		{
		    head1->a=0;
		    x=1;
		    break;
		}
		head1=head1->next;
	}
	if(x==1)
	merge(head);
	else
	cout<<"No such Memory Block Exists!!"<<endl;
	return head;
}
void Wanttofree()
{
	int a;
	cout<<"Want to free memory.\nPress any key else press 0 ";
	cin>>a;
	Node* head1;
	while(a!=0)
	{
		int fsize;
		cout<<"Enter size to be free=";
		cin>>fsize;
		head=free(head,fsize);
		Node* head1=head;
	    while(head1!=NULL)
	    {
		    cout<<"| "<<head1->size<<" "<<head1->fragment<<" "<<head1->a<<" |";
		    head1=head1->next;
	    }
	    cout<<endl;
		int x;
		cout<<"Want to free memory.\nPress any key else press 0 ";
	    cin>>x;
	    a=x;
	}
	head1=head;
	while(head1!=NULL)
    {
		cout<<"| "<<head1->size<<" "<<head1->fragment<<" "<<head1->a<<" |";
		head1=head1->next;
	}
	cout<<endl;
}
void FirstFit(int size)
{
	Node* head1=head;
	int x=0;
	int size1=size;
	if(size%8!=0)
	size1=size+8-size%8;
	while(head1!=NULL)
	{
		if(head1->size-size1>=8&&head1->a==0)
		{
			Node* newNode=head1->next;
			Node* temp=new Node(head1->size -size1,0);
			temp->next=newNode;
			head1->next=temp;
			head1->fragment=size1 -size;
			head1->size=size;
			head1->a=1;
			x=1;
			break;
		}
		else if(head1->size>=size1&&head1->a==0)
		{
			head1->fragment=8-size;
			head1->size=size;
			head1->a=1;
			x=1;
			break;
		}
		head1=head1->next;
	}
	if(x!=1)
	cout<<"Memory Full"<<endl;
	head1=head;
	while(head1!=NULL)
	    {
		    cout<<"| "<<head1->size<<" "<<head1->fragment<<" "<<head1->a<<" |";
		    head1=head1->next;
	    }
	    cout<<endl;
}
void BestFit(int size)
{
	Node* head1=head;
	int m=head->size;
	int size1=size;
	if(size%8!=0)
	size1=size+8-size%8;
	while(head1!=NULL)
	{
		if(head1->size>=size1&&head1->a==0)
		m=min(m,head1->size-size1);
		head1=head1->next;
	}
	int x=0;
	head1=head;
	while(head1!=NULL)
	{
		
		if(head1->size-size1==m&&head1->a==0)
		{
			if(head1->size-size1>=8&&head1->a==0)
		    {
			    Node* newNode=head1->next;
			    Node* temp=new Node(head1->size-size1,0);
		    	temp->next=newNode;
		    	head1->next=temp;
			    head1->fragment=size1-size;
			    head1->size=size;
			    head1->a=1;
			    x=1;
			    break;
		    }
		    else if(head1->size>=size1&&head1->a==0)
		    {
		    	head1->fragment=head1->size -size;
		    	head1->size=size;
		    	head1->a=1;
			    x=1;
		    	break;
		    }
		}
		head1=head1->next;
	}
	if(x!=1)
	cout<<"Memory Full"<<endl;
	head1=head;
	while(head1!=NULL)
	    {
		    cout<<"| "<<head1->size<<" "<<head1->fragment<<" "<<head1->a<<" |";
		    head1=head1->next;
	    }
	    cout<<endl;
}
int main(void)
{
	int heap;
	cout<<"Enter Size of Heap=";
	cin>>heap;
	HeapSize(heap);
	Wanttoallocate();
	Wanttofree();
	int a;
	//First Fit
	cout<<"Press any key to allocate else press 0 ";
	cin>>a;
	while(a!=0)
	{
		int size1;
		cout<<"Enter Block Size to allocate=";
		cin>>size1;
		FirstFit(size1);
		cout<<"Press any key to allocate again else press 0 ";
		int x;
		cin>>x;
		a=x;
	}
	Wanttofree();
	//Best Fit
	cout<<"Press any key to allocate else press 0 ";
	cin>>a;
	while(a!=0)
	{
		int size1;
		cout<<"Enter Block Size to allocate=";
		cin>>size1;
		BestFit(size1);
		cout<<"Press any key to allocate again else press 0 ";
		int x;
		cin>>x;
		a=x;
	}
	return 0;
}
