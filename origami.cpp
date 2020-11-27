/*-------------------------------------------------------------------------*
 *---									---*
 *---		origami.cpp						---*
 *---									---*
 *---									---*
 *---	    This file defines a program that showcases multi-threaded	---*
 *---	programming by simulates the creation and storage of origami	---*
 *---	art pieces.							---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a		2020 May 4		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

//	Compile with:
//	$ g++ origami.cpp -o origami -lpthread -g -std=c++11


//---	 		    File inclusion:	 			---//

#include		<stdlib.h>
#include		<stdio.h>
#include		<string.h>
#include		<unistd.h>	// sleep()
#include		<pthread.h>


//---	 			Constants:	 			---//

//  PURPOSE:  To tell how many origami pieces to make.
const int		NUM_PIECES_TO_MAKE	= 16;



//---	 			Classes:	 			---//

//  PURPOSE:  To hold information on an origami pattern.
class		Pattern
{
  //  I.  Member vars:
  //  PURPOSE:  To hold the name of the accessory;
  const char*			nameCPtr_;

  //  PURPOSE:  To hold the number of seconds it takes to fold.
  int				foldTimeInSecs_;

  //  II.  Disallowed auto-generated methods:
  //  No default constructor:
  Pattern			();

protected :
  //  III.  Protected methods:

public :
  //  IV.  Constructor(s), assignment op(s), factory(s) and destructor:
  //  PURPOSE:  To initialize '*this' to have name pointed to by 'newNameCPtr'
  //	and fold time 'newFoldTimeInSecs'.  No return value.
  Pattern			(const char*	newNameCPtr,
  				 int   		newFoldTimeInSecs
				) :
				nameCPtr_(newNameCPtr),
				foldTimeInSecs_(newFoldTimeInSecs)
				{ }

  //  PURPOSE:  To make '*this' a copy of 'source'.  No return value.
  Pattern			(const Pattern&	source
				) :
				nameCPtr_(source.getNameCPtr()),
				foldTimeInSecs_(source.getFoldTimeInSecs())
				{ }

  //  PURPOSE:  To resources of '*this', make '*this' a copy of 'source',
  //	and return a reference to '*this'.
  Pattern&	operator=	(const Pattern&	source
				)
				{
				  //  I.  Application validity chech:
				  if  (this == &source)
				  {
				    return(*this);
				  }

				  //  II.  Release resources:

				  //  III.  Copy 'source':
				  nameCPtr_	  = source.getNameCPtr();
				  foldTimeInSecs_ = source.getFoldTimeInSecs();

				  //  IV.  Finished:
				  return(*this);
				}

  //  PURPOSE:  To release the resources of '*this'.  No parameters.
  //	No return value.
  ~Pattern			()
				{ }

  //  V.  Accessors:
  //  PURPOSE:  To return the name of the accessory.  No parameters.
  const char*	getNameCPtr 	()
				const
				{ return(nameCPtr_); }

  //  PURPOSE:  To return the time it takes to fold '*this' pattern in seconds.
  //	No parameters.
  int		getFoldTimeInSecs
				()
				const
				{ return(foldTimeInSecs_); }

};


//  PURPOSE:  To hold the patterns.
const Pattern	patternArray[]
		= { {"flat paper",5},
		    {"Japanese crane",5},
		    {"Tyranosaus Rex",8},
		    {"paper airplane",5},
		    {"butterfly",8},
		    {"dragon",10},
		    {"parrot",12},
		    {"rabbit",15}
		  };

//  PURPOSE:  To hold the number of elements in 'patternArray[]':
const int	NUM_PATTERNS	= sizeof(patternArray)/sizeof(const Pattern);


//  PURPOSE:  To represent an origami piece.
class		Piece
{
  //  I.  Member vars:
  //  PURPOSE:  To keep track of the number of Piece instances that have been
  //	put away (destroyed).
  static
  int				count__;

  //  PURPOSE:  To hold the index of the pattern.
  int				patternIndex_;

  //  II.  Disallowed auto-generated methods:
  //  No copy assignment operator:
  Piece&	operator=	(const Piece&);

protected :
  //  III.  Protected methods:

public :
  //  IV.  Constructor(s),
  //  PURPOSE:  To make (cut) a flat piece of paper.  No parameters.
  // 	No return value.
  Piece				() :
				patternIndex_(0)
				{ }

  //  PURPOSE:  To make '*this' a copy of 'source'.  No return value.
  Piece				(const Piece&	source
  				) :
				patternIndex_(source.patternIndex_)
				{ }

  //  PURPOSE:  To release the resources of '*this'.  No parameters.
  //	No return value.
  ~Piece			()
				{
				  count__++;
				}

  //  V.  Accessors:
  //  PURPOSE:  To return the number of Piece instances that have been put away
  //	(destroyed).
  static
  int		getCount	()
  				{ return(count__); }

  //  PURPOSE:  To return the index of the pattern style.  No parameters.
  int		getPatternIndex()
				const
				{ return(patternIndex_); }

  //  PURPOSE:  To return a description of '*this' origami Piece instance as a
  //  	C-string.  No parameters.
  const char*	getDescriptionCPtr
				()
				const
				{ return(patternArray
						[getPatternIndex()].
						getNameCPtr()
					);
				}

  //  PURPOSE:  To return average time to make '*this' piece.  No parameters.
  int		getFoldTimeInSecs
				()
				const
				{ return(patternArray
						[getPatternIndex()].
						getFoldTimeInSecs()
					);
				}

  //  VI.  Mutators:
  //  PURPOSE:  To fold '*this' piece of paper.  No parameters.  No return
  //	value.
  void		fold		()
				{
				  patternIndex_	= (rand()%(NUM_PATTERNS-1)) + 1;
				  sleep(getFoldTimeInSecs());
				}

  //  VII.  Methods that do main and misc. work of class:

};


//  PURPOSE:  To hold a link in a linked list.
struct		Node
{
  //  I.  Member vars:
  //  PURPOSE:  To hold the address of an origami piece.
  Piece*			piecePtr_;

  //  PURPOSE:  To hold the address of the next link in the list, or 'NULL'
  //	if there is no such link.
  Node*				nextPtr_;

  //  II.  Constructor(s), assignment op(s), factory(s) and destructor:
  //  PURPOSE:  To make '*this' hold the address of piece 'newPiecePtr'.
  //	No return value.
  Node				(Piece*		newPiecePtr
				) :
				piecePtr_(newPiecePtr),
				nextPtr_(NULL)
				{ }

  //  PURPOSE:  To release the resources of '*this'.  No parameters.
  //	No return value.
  ~Node				()
  				{
				  // Do _not_ delete() 'nextPtr_'.
				  // '*this' does not own it.
				  // Do _not_ delete() 'piecePtr_'.
				  // '*this' does not own it.
				}

  //  III.  Accessors:
  //  PURPOSE:  To return the address of an origami piece.  No parameters.
  Piece*	getPiecePtr   	()
				const
				{ return(piecePtr_); }

  //  PURPOSE:  To return the address of the next link in the list, or 'NULL'
  //	if there is no such link.  No parameters.
  Node*		getNextPtr  	()
  				const
				{ return(nextPtr_);}


  //  IV.  Mutators:
  //  PURPOSE:  To set the address of the origami piece to 'newPiecePtr'.
  //	No return value.
  void		setPiecePtr   	(Piece*		newPiecePtr
				)
				{ piecePtr_	= newPiecePtr; }

  //  PURPOSE:  To set the address of the next node to 'newNextPtr'.
  //	No return value.
  void		setNextPtr   	(Node*		newNextPtr
				)
				{ nextPtr_	= newNextPtr; }

};


//  PURPOSE:  To store origami art works.
class		Table
{
  //  I.  Member vars:
  //  PURPOSE:  To tell the current length of the linked list.
  size_t			currentLen_;

  //  PURPOSE:  To tell the maximum length of the linked list.
  size_t			maxLen_;

  //  PURPOSE:  To point to the beginning of the linked list.
  Node*				beginPtr_;

  //  PURPOSE:  To point to the end of the linked list.
  Node*				endPtr_;

  //  PURPOSE:  To hold the mutex used to control access to '*this'.
  pthread_mutex_t		lock_;

  //  PURPOSE:  To hold the condition to signal when '*this' is no longer full.
  pthread_cond_t		notFull_;

  //  PURPOSE:  To hold the condition to signal when '*this' is no longer empty.
  pthread_cond_t		notEmpty_;

  //  II.  Disallowed auto-generated methods:
  //  No default constructor:
  Table				();

  //  No copy constructor:
  Table				(const Table&);

  //  No copy assignment op:
  Table&	operator=	(const Table&);

protected :

public :
  //  IV.  Constructors:
  //  PURPOSE:  To initialize '*this' to an empty store of addresses of up to
  //	'newMaxLen' Piece instances.  No return value.
  Table				(size_t		newMaxLen
  				) :
				currentLen_(0),
				maxLen_(newMaxLen),
				beginPtr_(NULL),
				endPtr_(NULL)
  				{
				  pthread_mutex_init(&lock_,NULL);
				  pthread_cond_init(&notFull_,NULL);
				  pthread_cond_init(&notEmpty_,NULL);
				}

  //  PURPOSE:  To release the resources of '*this'.  No parameters.  No
  //	return value.
  ~Table			()
				{
				  if  (getNumItems() > 0)
				  {
				    printf("Too bad: we have to "
				    	   "throw these nice pieces away:\n"
					  );

				    do
				    {
				      delete(pullOut(""));
				    }
				    while  (getNumItems() > 0);
				  }

				  pthread_cond_destroy(&notEmpty_);
				  pthread_cond_destroy(&notFull_);
				  pthread_mutex_destroy(&lock_);
				}

  //  V.  Accessors:
  //  PURPOSE:  To return the number of items in '*this' store.  No parameters.
  int		getNumItems	()
  				const
				{
				  return(currentLen_);
				}

  //  PURPOSE:  To return the maximum length of the linked list.
  size_t	getMaxLen	()
				const
				{ return(maxLen_); }

  //  VI.  Mutators:

  //  VII.  Methods that do misc. and main work of class:
  //  PURPOSE:  To place 'piecePtr' in '*this' store.  Prints out
  //	'waitingMsgCPtr' if forced to wait before doing so.  No return value.
  void		putIn		(Piece*		piecePtr,
  				 const char*	waitingMsgCPtr
				)
  {
    //  I.  Applicability check:

    //  II.  Place 'piecePtr' in '*this':
    pthread_mutex_lock(&lock_);

    while  (getNumItems() >= getMaxLen())
    {
      printf("%s",waitingMsgCPtr);
      pthread_cond_wait(&notFull_,&lock_);
    }

    Node*	nodePtr	= new Node(piecePtr);

    if  (beginPtr_ == NULL)
    {
      beginPtr_	= nodePtr;
    }
    else
    {
      endPtr_->setNextPtr(nodePtr);
    }

    endPtr_	= nodePtr;
    currentLen_++;

    pthread_cond_signal(&notEmpty_);
    pthread_mutex_unlock(&lock_);

    //  III.  Finished:
  }


  //  PURPOSE:  To get and return the address of an origami Piece instance
  //	from '*this' store.  Prints out 'waitingMsgCPtr' if forced to wait
  //	before doing so.
  Piece*	pullOut		(const char*	waitingMsgCPtr
				)
  {
    //  I.  Applicability check:

    //  II.  Place 'piecePtr' in '*this':
    pthread_mutex_lock(&lock_);

    while  (getNumItems() == 0)
    {
      printf("%s",waitingMsgCPtr);
      pthread_cond_wait(&notEmpty_,&lock_);

      if  (Piece::getCount() >= NUM_PIECES_TO_MAKE)
      {
        return(NULL);
      }
    }

    Piece*	toReturn	= beginPtr_->getPiecePtr();
    Node*	nodePtr		= beginPtr_->getNextPtr();

    delete(beginPtr_);
    beginPtr_			= nodePtr;

    if  (beginPtr_ == NULL)
    {
      endPtr_	= NULL;
    }

    currentLen_--;

    pthread_cond_signal(&notFull_);
    pthread_mutex_unlock(&lock_);

    //  III.  Finished:
    return(toReturn);
  }


  void		wakeUpAllWaiters	()
  {
    pthread_cond_broadcast(&notEmpty_);
  }
};


struct		Room
{
  Table*			paperTablePtr_;
  Table*			finishedTablePtr_;

  Room				(Table*	ptr0,
				 Table*	ptr1
				)
				{
				  paperTablePtr_	= ptr0;
				  finishedTablePtr_	= ptr1;
				}
};


//  PURPOSE:  To keep track of the number of origami Piece instances that have
//	been put away (destroyed).
int		Piece::count__	= 0;


//  PURPOSE:  To make 'NUM_PIECES_TO_MAKE' pieces of paper instances and put
//	their addresses in the first Table.
void* 		paperCutter	(void*		vPtr
				)
{
  Piece*	piecePtr;
  Table*	tablePtr	= (Table*)vPtr;

  for  (size_t i = 0;  i < NUM_PIECES_TO_MAKE;  i++)
  {
    printf("Paper cutter: \"Time to cut more paper.\"\n");
    piecePtr	= new Piece();
    printf("Paper cutter: \"There you go, another sheet of paper.\"\n");
    tablePtr->
	putIn(piecePtr,
	      "Paper cutter: \"Hey, get folding!  The paper table is full\"\n"
	     );
  }

  printf("Paper cutter: \"I'm done.  And I still have all my fingers!\"\n");
  return(NULL);
}


//  PURPOSE:  To take 'NUM_PIECES_TO_MAKE/2' Piece instances from the first 
//	table, fold them, and put them on the second table.
void* 		paperFolder	(void*		vPtr
				)
{
  Piece*	piecePtr;
  Room*		roomPtr		= (Room*)vPtr;
  Table*	paperTablePtr	= roomPtr->paperTablePtr_;
  Table*	secondStorePtr	= roomPtr->finishedTablePtr_;

  for  (size_t i = 0;  i < NUM_PIECES_TO_MAKE/2;  i++)
  {
    piecePtr	= paperTablePtr->
			pullOut("Folder \"Hey paper cutter, don't fall"
				" asleep with scissors in your hand!\"\n"
			       );

    piecePtr->fold();
    printf("Folder \"I just made a %s\"\n",piecePtr->getDescriptionCPtr());
    secondStorePtr->
	putIn(piecePtr,"Folder \"Hey stocker, get to work!\"\n");
  }

  printf("Folder: \"Whew!  That was a lot of paper to fold!\"\n");
  return(NULL);
}


//  PURPOSE:  To obtain addresses of Piece instances from the second table,
//	and put them away (destroy them).
void* 		stocker		(void*		vPtr
				)
{
  Piece*	piecePtr;
  Table*	tablePtr	= (Table*)vPtr;

  while  (Piece::getCount() < NUM_PIECES_TO_MAKE)
  {
    piecePtr	= tablePtr->pullOut("Stocker \"Those lazy folders!\"\n");

    if  (piecePtr == NULL)
    {
      break;
    }

    printf("Stocker \"That is a very nice %s. I will put it away.\"\n",
	   piecePtr->getDescriptionCPtr()
	  );
    sleep(6);
    delete(piecePtr);
  }

  printf("Stocker: \"Lifting all those boxes was tiring, time to go!\"\n");
  return(NULL);
}



int		main		(int		argc,
				 char*		argv[]
				)
{
  Table			paperTable(2);
  Table			finishedTable(NUM_PIECES_TO_MAKE/4);
  Room			room(&paperTable,&finishedTable);
  pthread_t		cutterId;
  pthread_t		folderId[2];
  pthread_t		storerId;

  srand(getpid());

  pthread_create(&cutterId,NULL,paperCutter,(void*)&paperTable);
  pthread_create(&folderId[0],NULL,paperFolder,&room);
  pthread_create(&folderId[1],NULL,paperFolder,&room);
  pthread_create(&storerId,NULL,stocker,&finishedTable);

  while  ( Piece::getCount() < NUM_PIECES_TO_MAKE)
  {
    sleep(1);
  }

  //store.wakeUpAllWaiters();
  pthread_join(cutterId,NULL);
  pthread_join(folderId[0],NULL);
  pthread_join(folderId[1],NULL);
  pthread_join(storerId,NULL);

  return(EXIT_SUCCESS);
}
