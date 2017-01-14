#include<iostream>
#include<time.h>
#include<stdlib.h>
using namespace std;

struct node					//Defining a node
{
	int win,draw,loss;
	int num;
	node* next[9];
}*root=new node;

void initializeroot()		//Initializing a node
{
	root->win=0;
	root->draw=0;
	root->loss=0;
	for(int i=0;i<9;i++)
		root->next[i]=NULL;
}


node* create(int number)		//creating a new node dynamically
{
	node* newnode= new node;
	newnode->num= number;
	newnode->win=0;
	newnode->draw=0;
	newnode->loss=0;

	for(int child=0;child<9;child++)
		newnode->next[child]=NULL;
	return newnode;
}

void makethetree(node* temp, int level)		//creating a 3X3 tree
{
	if(level==8)							//level0- root
			return;
	for(int child=0;child<9;child++)
	{
		temp->next[child]=create(child);		// tree is created in order similar to preorder traversal
		makethetree(temp->next[child], level+1);
	}
}

void preorder(node*temp)						//prerorder traversal of tree to print num
{
	cout<<temp->num<<"["<<temp->draw<<":"<<temp->win<<":"<<temp->loss<<"]"<<endl;;
	for(int child=0; child<9; child++)
	{
		if(temp->next[child]!=NULL)
			preorder(temp->next[child]);
	}
}

//fitting the tree with set of moves (randomly generated )

void fit(node* temp, int sequence[], int size, int result, int accesser)	// look comment of ramnumgen, accesser iterates over arr till size
{
	if(accesser<size)
	{
		int x=sequence[accesser];
		temp=temp->next[x];								//temp is equal to xth child

		if(result==0)									//incrementing win/draw/loss according to result
		temp->draw++;
		else if(result==1)
		temp->win++;
		else if(result==2)
		temp->loss++;
		else
		cout<<"Error in result"<<endl;

		accesser++;										//increment accesser

		fit(temp, sequence, size, result, accesser);

	}
}


int check(int sequence[], int size)		// Inputs sequence of moves in order
{										//returns 0->draw, 1->player1 wins, 2->player2 wins, -1-> game still on
	int mat[3][3],i,j;					//mat is 3X3 representation of different moves by player
										//mat consists only of 0-unoccupied, 1-occupied by player1 and 2-occupied by player2

	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
			mat[i][j]=0;
	}

	for(i=0;i<size;i++)					// converting sequence of moves to 3X3 representation of 0,1 and 2
	{
		int x= sequence[i];
		int quotient= x/3;
		int remainder= x%3;
		if(i%2==0)
			mat[quotient][remainder]=1;
		else
			mat[quotient][remainder]=2;
	}

	for(i=0;i<3;i++)					//checking each row for possible win
	{
		if( (mat[i][0]==mat[i][1])&&(mat[i][1]==mat[i][2])&&(mat[i][0]!=0) )
		return mat[i][0];
	}

	for(i=0;i<3;i++)					////checking each column for possible win
	{
		if( (mat[0][i]==mat[1][i])&&(mat[1][i]==mat[2][i])&&(mat[0][i]!=0) )
		return mat[0][i];
	}

	if( (mat[0][0]==mat[1][1])&&(mat[1][1]==mat[2][2])&&(mat[0][0]!=0) )	//main diagonal check
		return mat[0][0];

	if( (mat[0][2]==mat[1][1])&&(mat[1][1]==mat[2][0])&&(mat[0][2]!=0) )	//other diagonal check
		return mat[0][2];

	if(size==9)			// true when neither of player win and total moves is 9 ie. draw
		return 0;

	return -1;			//game is still on...


}

void generate(int sequence[], int &size, int &result)	//to generate valid sequence of moves
{
	int count=0, gen_num;
	bool flag;
	do
	{
		flag=true;
		while(flag==true)					// false when a new move which hasn't been played earlier is generated
		{
			flag=false;
			gen_num=rand()%9;				//generate any random number;range:0-8
			for(int i=0;i<count;i++)
			{
				if(gen_num==sequence[i])	//check: true when the move is played earlier
					flag=true;

			}
		}
	sequence[count]=gen_num;				//assign a new move when the move isn't played earlier
	count++;
	result=check(sequence, count);
	}while(result==-1);					//Loop ends when either of player wins or draw

	size=count;

}

													//sequence is the set of moves already played
int nextmove(node* temp, int* sequence, int size, int from_player)	//temp is root when the function is called first time
{													//temp then  equals to one of its children as modified in main()
	node* children[9];
	int i, j, move, highest_win;
	for(i=0;i<9;i++)
	{
		children[i]=temp->next[i];
	}
	cout<<endl;


	for(i=0;i<9;i++)					//checks if the move is played earlier or not
	{
		bool flag=false;
		for(j=0;j<size;j++)
			if(i==sequence[j])
				flag=true;
		if(flag==false)					//If move isn't played earlier then assign it to int move as initialization
		{
			move=i;
			if(from_player==1)
			{
				highest_win= ((1*children[i]->draw)+(2*children[i]->win)-(2*children[i]->loss));
			}
			else if(from_player==2)
			{
				highest_win= ((2*children[i]->draw)+(2*children[i]->loss)-(3*children[i]->win));
			}


			break;
		}
	}

	for(i=0;i<9;i++)					//checks if the move is played earlier or not
	{
		bool flag=false;
		for(j=0;j<size;j++)
			if(i==sequence[j])			//If the move has been played earlier then flag=true
				flag=true;
		if(flag==false)					//condition is True when a move isn't played earlier
		{
			if(from_player==1)
			{
				if(highest_win< ((1*children[i]->draw)+(2*children[i]->win)-(2*children[i]->loss)))	//if there some else move has higher probability,
				{									//assign int move to that move
					move=i;
					highest_win= ((1*children[i]->draw)+(2*children[i]->win)-(2*children[i]->loss));
				}
			}
			else if(from_player==2)
			{
				if(highest_win< ((2*children[i]->draw)+(2*children[i]->loss)-(3*children[i]->win)))	//if there some else move has higher probability,
				{									//assign int move to that move
					move=i;
					highest_win= ((2*children[i]->draw)+(2*children[i]->loss)-(3*children[i]->win));
				}
			}
		}
	}

	return move;						//return the new move played by the computer
}

void display(int sequence[], int size)
{
	char mat[3][3];
	int i,j;				//mat is 3X3 representation of different moves by player
											//mat consists only of 0-unoccupied, 1-occupied by player1 and 2-occupied by player2

	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			switch(3*i+j)			// Converting numbers into characters
			{
			case 0: mat[i][j]='0';
					break;
			case 1: mat[i][j]='1';
					break;
			case 2: mat[i][j]='2';
					break;
			case 3: mat[i][j]='3';
					break;
			case 4: mat[i][j]='4';
					break;
			case 5: mat[i][j]='5';
					break;
			case 6: mat[i][j]='6';
					break;
			case 7: mat[i][j]='7';
					break;
			case 8: mat[i][j]='8';
					break;
			default:
					cout<<"Display error"<<endl;
					break;

			}
		}

	}

	for(i=0;i<size;i++)					// converting sequence of moves to 3X3 representation of numbers, 'x' and 'o'
	{
		int x= sequence[i];
		int quotient= x/3;
		int remainder= x%3;
		if(i%2==0)
			mat[quotient][remainder]='x';
		else
			mat[quotient][remainder]='o';
	}

	for(i=0;i<3;i++)							//styling
	{
		for(j=0;j<3;j++)
		{
			cout<<"|"<<mat[i][j]<<"|";
		}
		cout<<endl;
	}
}


int main()
{
	int size, trash, result, sequence[9], player1move, player2move, chooseplayer;	//to be passed to rannumgen function
	int trainnum;								//no. of times to train the tree with random numbers
	int play_again=1;
	initializeroot();
	node*temp= root;

	cout<<"Hey, I am Nikhil :) .Lets play tic tac toe!!"<<endl;
	cout<<"This program lets computer train itself to win the game!!"<<endl;
	cout<<"Press any button to continue..."<<endl;
	cin>>trash;

	cout<<"[Status]: Constructing the tree..."<<endl;
	cout<<"This may take few minutes..."<<endl;
	makethetree(temp,1);

	srand(time(NULL));
	cout<<"Enter the number of times you want to train the tree with random sequence of moves"<<endl;
	cout<<"Recommended times: 10^8 (usually takes upto 4 mins to train)"<<endl;
	cin>>trainnum;
	cout<<"This might take few minutes"<<endl;
	cout<<"[Status]: Fitting the tree..."<<endl;
	while(trainnum--)							//training the tree
	{
		size=0;
		temp=root;
		generate(sequence, size, result);
		if(size>7)
			size=7;
		fit(temp,sequence,size,result, 0);		//fitting the tree with moves
	}


	while(play_again!=0)						//True when user wants to play again
	{
		temp=root;
		size=0;

		do
		{
			cout<<"Press"<<endl<<"1 to be player-1 (x)"<<endl<<"2 to be player-2 (o)"<<endl;
			cin>>chooseplayer;
			if((chooseplayer!=1)&&(chooseplayer!=2))
				cout<<"Enter either 1 or 2"<<endl;
		}while((chooseplayer!=1)&&(chooseplayer!=2));

		/////////////////////////////////// When user chooses to be player 2

		if(chooseplayer==2)
		{
			while(check(sequence,size)==-1)				//Predicting moves
			{
				bool flag= true;
				player1move=nextmove(temp, sequence, size, 1);	//Move predicted based on probabilistic formula
				cout<<"Computer played: "<<player1move<<endl;

				sequence[size]=player1move;					//storing the move in sequence
				size++;
				temp=temp->next[player1move];				//traversing the tree from parent to one of its
															//children based on the move played
				display(sequence,size);

				if(check(sequence,size)!=-1)				//condition is true when the game has ended
					break;

				while(flag==true)							//condition is true when player2move has already
				{											//been played
					flag=false;
					cout<<"Enter the next move"<<endl;
					cin>>player2move;
					for(int i=0;i<size;i++)
					{
						if(player2move==sequence[i])		//checking if the move has already been played
						{
							flag= true;
							cout<<"Repetition of move not allowed"<<endl;
							break;
						}
					}
				}
				sequence[size]=player2move;		//storing the new move in sequence
				size++;
				temp=temp->next[player2move];	//traversing the tree from parent to one of its
				display(sequence,size);								//children based on the move played
			}
		}

		/////////////////////////////////// When user chooses to be player 1

		else
		{
			display(sequence, size);
			while(check(sequence,size)==-1)				//Predicting moves
			{
				bool flag= true;
				while(flag==true)							//condition is true when player2move has already
				{											//been played
					flag=false;
					cout<<"Enter the next move"<<endl;
					cin>>player2move;
					for(int i=0;i<size;i++)
					{
						if(player2move==sequence[i])		//checking if the move has already been played
						{
							flag= true;
							cout<<"Repetition of move not allowed"<<endl;
							break;
						}
					}
				}
				sequence[size]=player2move;		//storing the new move in sequence
				size++;
				temp=temp->next[player2move];	//traversing the tree from parent to one of its
														//children based on the move played
				display(sequence,size);

				if(check(sequence,size)!=-1)				//condition is true when the game has ended
					break;

				player1move=nextmove(temp, sequence, size, 2);	//Move predicted based on probabilistic formula
				cout<<"Computer played: "<<player1move<<endl;

				sequence[size]=player1move;					//storing the move in sequence
				size++;
				temp=temp->next[player1move];				//traversing the tree from parent to one of its
																//children based on the move played
				display(sequence,size);
			}

		}

		if(check(sequence,size)==0)
			cout<<"Match Draw"<<endl;
		else if(check(sequence,size)==1)
			cout<<"Player 1 won"<<endl;
		else if(check(sequence, size)==2)
			cout<<"Player 2 won";
		else
			cout<<"Error in player number"<<endl;

		cout<<endl<<"Press 0 to exit and any other number to play again"<<endl;
		cin>>play_again;

	}
cout<<"Hope you enjoyed playing, thanks!"<<endl;

	return 0;
}



