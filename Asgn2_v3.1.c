/*-------------------------------------------------------------------
*/
/* ITS60304– Assignment #2 */
/* C Programming */
/* Student Name: <Tan Vi Lynn> <Heng Hian Wee> */
/* Student ID: <0320951> <0321343> */
/*-------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LOWSTOCK 5

// all function prototypes
int opt1_purchase( void );
int opt2_update( void );
int opt3_delete ( void );
void opt4_showinventory( void );

typedef struct Book {
	double bISBN; // book ISBN
	char bTitle[ 100 ]; // book title
	double bPrice; // book price
	int bQ; // book quantity
	int bDis; // book discount
} Book;

typedef struct DailyTransactions {
	int tbs; // total books sold
	double tRev; // total revenue
	double dis; // total discount
} DailyTransactions;

DailyTransactions dt = {0}; // global declaration for daily transactions
int counter = 1; // book counter

// main function
int main( void )
{
	char temp;
	int menu_opt; // menu option
	FILE *bPtrR; // file pointer (read bookstore.txt)

	// counts no. of different types of books available
	counter = 1; // book counter always reset to 1
	bPtrR = fopen( "bookstore.txt", "r" );
	if( bPtrR != NULL ) // check for existence of bookstore.txt
	{
		while( !feof( bPtrR ) )
		{
			temp = fgetc( bPtrR );
			if( temp == '\n' ) // Once the end of line is read,
				counter++; // count the number of different types of books available
		} // end while
	}
	else
		printf( "File not found.\n" );
	// end if-else
	fclose( bPtrR );

	do
	{
		// main menu interface
		printf( "\n------------------------------------" );
		printf( "\nGrocery Retail" );
		printf( "\n------------------------------------" );
		printf( "\n\t1. Purchase book" );
		printf( "\n\t2. Update book" );
		printf( "\n\t3. Delete book" );
		printf( "\n\t4. Show inventory" );
		printf( "\n\t5. Show daily transaction" );
		printf( "\n\t6. Exit" );

		printf( "\n\nEnter an option: " );
		scanf( "%d", &menu_opt ); // retrieve user selected option

		switch( menu_opt )
		{
			case 1: // <Purchase book> selected
				opt1_purchase();
				break;

			case 2: // <Update book> selected
				opt2_update();
				break;

			case 3: // <Delete book> selected
				opt3_delete();
				break;

			case 4: // <Show inventory> selected
				opt4_showinventory();
				break;

			case 5: // <Show daily transactions> selected

				printf( "\n\nDaily Transactions" );
				printf( "\n------------------------------------" );
				printf( "\nTotal books sold         : %d", dt.tbs );
				printf( "\nTotal revenue            : RM %.2lf", dt.tRev );
				printf( "\nDiscount                 : RM %.2lf\n\n", dt.dis );
				break;

			default: // other option (not 6) entered
				if( menu_opt < 1 || menu_opt > 6 )
					printf( "Invalid option. Please try again.\n" );
		} // end switch (menu_opt)
	} while( menu_opt != 6 ); // if menu_opt6 == 6, program terminates

	// notifies user that program termination is successful
	printf( "\nProgram status: Terminated." );
	printf( "\nThank you for using this program. :)" );
	printf( "\n" );

	return 0;
} // end main


// <Purchase book> function
int opt1_purchase( void )
{
	Book book[ counter ];
	double uISBN; // user input ISBN
	int i = 0, k;
	int bnf = 0, uQ; // book not found; user input quantity
	double tPrice = 0; // total price = 0
	double tDis = 0; // total discount = 0
	FILE *bPtrR; // file pointer (read bookstore.txt)
	FILE *bPtrW; // file pointer (write bookstore.txt)
	FILE *pPtr; // file pointer (purchase.txt)

	// retrieve and store books in arrays
	bPtrR = fopen( "bookstore.txt", "r" );
	while( !feof( bPtrR ) )
	{
		fscanf( bPtrR, "%lf:%[^:]:%lf:%d:%d\n", &book[i].bISBN, book[i].bTitle, &book[i].bPrice, &book[i].bQ, &book[i].bDis );
		i++;
	} // end while
	fclose( bPtrR );

	// prompt for ISBN of book to be purchased
	printf( "\nEnter the 13-digit ISBN(Enter '-1' to indiate there is no book to be purchased): " );
	scanf( "%13lf", &uISBN ); // retrieve 13 ISBN digits

	if( uISBN == -1 ) // check if there is no book to be purchased
		return 0;
	else
	{
		// checks if book is under promotion
		for( i = 0; i < counter; i++ )
			if( uISBN == book[i].bISBN )
				if( book[i].bDis != 0 )
					printf( "Good news, this book is %d%% off!!\n", book[i].bDis );

		printf( "Enter the quantity per book: " );
		scanf( "%d", &uQ ); // retrieve user input quantity

		if( uQ != 0 ) // check if there is no book to be purchased
		{
			for( i = 0; i < counter; i++ )
			{
				if( uISBN == book[i].bISBN ) // check if book exists
				{
					if( uQ > book[i].bQ ) // check if user requests quantity more than stock
					{
						// notfies user that stock < quantity
						printf( "\nThe amount of books you requested is currently not available." );
						printf( "\nMaximum amount of books available: %d\n", book[i].bQ );
						break;
					}
					else
					{
						tDis = (double)book[i].bDis / 100 * book[i].bPrice * uQ;
						tPrice = (double)book[i].bPrice * uQ - tDis;

						book[i].bQ -= uQ; // remaining stock left

						// write purchased to purchase.txt
						pPtr = fopen( "purchase.txt", "a+" );
						fprintf( pPtr, "%13.0lf;%s;%.2lf;%d\n", book[i].bISBN, book[i].bTitle, book[i].bPrice, uQ );
						fclose( pPtr );

						// update daily transactions
						dt.tbs += uQ; // add user quantity to total books sold
						dt.tRev += tPrice; // add total price to total revenue
						dt.dis += tDis; // add total discount to discount

						// receipt interface
						printf( "\n\n********************RECEIPT*********************\n\n" );
						printf( "ISBN:                      %13.0lf\n", book[i].bISBN );
						printf( "Book title:                %s\n", book[i].bTitle );
						printf( "Book Price:                RM %.2lf\n", book[i].bPrice );
						printf( "Quantity:                  %d\n", uQ );
						printf( "TOTAL(after discount):     RM %.2lf(-%.2lf)", tPrice, tDis );
						printf( "\n\n********************RECEIPT*******************\n\n" );
					} // end if-else ( uQ > book[i].bQ )
				}
				else
					bnf++; // if( uISBN != bISBN[i] ), add 1 to book not found
				// end if-else ( uISBN = book[i].bISBN )

			} // end for( i = 0; i < counter; i++ )
		} 
		else
			return 0;
		// end if-else ( uQ != 0 )
	} // end if-else ( uISBN != -1 )

	if( bnf == counter ) // all books are scanned but book not found
		printf( "\nInvalid item.\n" );
	else
	{
		// update bookstore.txt
		bPtrW = fopen( "bookstore.txt", "w" );
		for( i = 0; i < ( counter - 1 ); i++ )
			fprintf( bPtrW, "%13.0lf:%s:%.2lf:%d:%d\n", book[i].bISBN, book[i].bTitle, book[i].bPrice, book[i].bQ, book[i].bDis );

		if( i == ( counter -1 ) )
			fprintf( bPtrW, "%13.0lf:%s:%.2lf:%d:%d\0", book[i].bISBN, book[i].bTitle, book[i].bPrice, book[i].bQ, book[i].bDis );
		fclose( bPtrW );
	} // end if-else ( bnf == counter )
} // end <Purchase book>

// <Update book> function
int opt2_update( void )
{
	Book book[ counter ];
	char nTitle[ 100 ]; // new book title
	int nQ, nDis; // new quantity, new discount
	double nPrice; // user input ISBN
	char uConfirm; // user confirmation
	int uChoice; // user choice
	double uISBN; // user input ISBN
	int i = 0, j, k;
	int bnf = 0; // book not found
	FILE *bPtrR; // file pointer (read bookstore.txt)
	FILE *bPtrW; // file pointer (write bookstore.txt)

	// retrieve and store books in arrays
	bPtrR = fopen( "bookstore.txt", "r" );
	while( !feof( bPtrR ) )
	{
		fscanf( bPtrR, "%lf:%[^:]:%lf:%d:%d\n", &book[i].bISBN, book[i].bTitle, &book[i].bPrice, &book[i].bQ, &book[i].bDis );
		i++;		
	} // end while
	fclose( bPtrR );

	// prompt for ISBN of book to be updated
	printf( "\nEnter the 13-digit ISBN(Enter '-1' to indicate there is no book to be updated): " );
	scanf( "%13lf", &uISBN ); // retrieve 13 ISBN digits

	if( uISBN == -1 ) // check if there is no book to be updated
		return 0;
	else
	{
		for( i = 0; i < counter; i++ )
		{
			if( uISBN == book[i].bISBN ) // check if book exists
			{
				j = i; // mark 'j'th item in bookstore.txt

				printf( "\n------------------------------------" );
				printf( "\nUpdate Directory: " );
				printf( "\n------------------------------------" );
				printf( "\n\t1. Book title" );
				printf( "\n\t2. Price" );
				printf( "\n\t3. Quantity" );
				printf( "\n\t4. Discount" );

				// prompt for field to be changed
				printf( "\n\nEnter field to be updated(Enter '-1' to indicate there is no field to be updated): " );
				scanf( "%d", &uChoice );

				if( uChoice == -1 ) // check if there is no field to be updated
					return 0;
				else
				{
					switch( uChoice )
					{
						case 1: // book title update
							printf( "Enter new book title for %.0lf : ", book[i].bISBN );
							getchar();
							scanf( "%99[^\n]", nTitle );
							break;

						case 2: // book price update
							printf( "Enter new book price for %.0lf : ", book[i].bISBN );
							scanf( "%d", &nPrice );
							break;

						case 3: // book quantity update
							printf( "Enter new quantity for %.0lf : ", book[i].bISBN );
							scanf( "%d", &nQ );
							break;

						case 4: // book discount update
							printf( "Enter new discount(%%) for %.0lf : ", book[i].bISBN );
							scanf( "%d", &nDis );
							break;

						default:
							if( uChoice < 1 || uChoice > 4 )
								printf( "Invalid field option.\n" );
					} // end switch (uChoice)
				} // end if-else ( uChoice == -1 )
			}
			else
				bnf++; // if ( uISBN != bISBN[i] ), add 1 to book not found
			// end if-else ( uISBN == book[i].bISBN )
		} // end for( i = 0; i < counter; i++ )

		if( bnf == counter ) // all books are scanned but book not found
			printf( "\nBook not found.\n" );
		else
		{
			// prompt confirmation to update
			printf( "Confirm update? (Y/N) : " );
			scanf( " %c", &uConfirm ); // retrieve confirmation

			if( uConfirm == 'Y' || uConfirm == 'y' ) // if update is confirmed
			{
				if( uChoice == 1 )
					strcpy( book[j].bTitle, nTitle );
				else if( uChoice == 2 )
					book[j].bPrice = nPrice;
				else if( uChoice == 3 )
					book[j].bQ = nQ;
				else if( uChoice == 4 )
					book[i].bDis = nDis;
				else
					printf( "\nError.\n" ); // error notification

				// update bookstore.txt
				bPtrW = fopen( "bookstore.txt", "w" );
				for( k = 0; k < counter; k++ )
				{
					if( k == ( counter - 1 ) )
						fprintf(bPtrW, "%13.0lf:%s:%.2lf:%d:%d\0", book[k].bISBN, book[k].bTitle, book[k].bPrice, book[k].bQ, book[k].bDis);
					else
						fprintf(bPtrW, "%13.0lf:%s:%.2lf:%d:%d\n", book[k].bISBN, book[k].bTitle, book[k].bPrice, book[k].bQ, book[k].bDis);
				} // end for ( k = 0; k < counter; k++ )
				fclose( bPtrW );

				// successful notification
				printf( "\nYou have successfully updated the book with the ISBN %.0lf.\n", book[j].bISBN );
			}
			else if( uConfirm == 'N' || uConfirm == 'n' ) // update to be discarded
				return 0;
			else // invalid input
				printf( "\nError.\n" ); // error notification
			// end if-else (uConfirm)
		} // end if-else( bnf == counter )
	} // end if-else ( uISBN == -1 )
} // end <Update book>


// <Delete book> function
int opt3_delete( void )
{
	Book book[ counter ];
	char uConfirm; // user input confirmation
	int i = 0, j, k, m;
	int bnf = 0; // book not found
	double uISBN; // user input ISBN
	FILE *bPtrR; // file pointer (read bookstore.txt)
	FILE *bPtrW; // file pointer (write bookstore.txt)

	// retrieve and store books in arrays
	bPtrR = fopen( "bookstore.txt", "r" );
	while( !feof( bPtrR ) )
	{
		fscanf( bPtrR, "%lf:%[^:]:%lf:%d:%d\n", &book[i].bISBN, book[i].bTitle, &book[i].bPrice, &book[i].bQ, &book[i].bDis ); // retrieve and store books in arrays
		i++;
	} // end while
	fclose( bPtrR );

	printf( "\nEnter the 13-digit ISBN(Enter '-1' to indicate there is no book to be deleted): " );
	scanf( "%13lf", &uISBN ); // retrieve 13 ISBN digits

	if( uISBN == -1 ) // check if there is no book to be deleted
		return 0;
	else
	{
		for( i = 0; i < counter; i++ )
		{
			if( uISBN == book[i].bISBN ) // check if book exists
			{
				if( book[i].bQ == 0 )
					j = i; // mark 'j'th item in bookstore.txt
				else
				{
					printf( "You cannot delete a book which is still in stock.\n" ); // books can only be deleted if quantity == 0
					return 0;
				} // end if-else ( book[i].bQ == 0 )
			} 
			else
				bnf++; // if( uISBN != bISBN[i] ), add 1 to book not found
			// end if-else ( uISBN == book[i].bISBN )
		} // end for ( i = 0; i < counter; i++ )
	} // end if-else ( uISBN == -1 )

	if( bnf == counter ) // all books are scanned but book not found
		printf( "\nBook not found.\n" );
	else
	{
		// prompt confirmation to delete
		printf( "Confirm delete? (Y/N): " );
		scanf( " %c", &uConfirm ); // retrieve confirmation

		if( uConfirm == 'Y' || uConfirm == 'y' ) // if delete is confirmed
		{
			// update bookstore.txt
			bPtrW = fopen( "bookstore.txt", "w" );
			if( j == 0 ) // delete first record
			{
				for( k = 1; k < counter; k++ )
				{
					if( k == ( counter - 1 ) )
						fprintf( bPtrW, "%13.0lf:%s:%.2lf:%d:%d\0", book[k].bISBN, book[k].bTitle, book[k].bPrice, book[k].bQ, book[k].bDis );
					else
						fprintf( bPtrW, "%13.0lf:%s:%.2lf:%d:%d\n", book[k].bISBN, book[k].bTitle, book[k].bPrice, book[k].bQ, book[k].bDis );
				} // end for ( k = 1; k < counter; k++ )
			}
			else if( j == (counter - 1) ) // delete last record
			{
				for( k = 0; k < ( counter - 1 ); k++ )
				{
					if( k == ( counter - 2) )
						fprintf( bPtrW, "%13.0lf:%s:%.2lf:%d:%d\0", book[k].bISBN, book[k].bTitle, book[k].bPrice, book[k].bQ, book[k].bDis );
					else
						fprintf( bPtrW, "%13.0lf:%s:%.2lf:%d:%d\n", book[k].bISBN, book[k].bTitle, book[k].bPrice, book[k].bQ, book[k].bDis );
				} // end for ( k = 0; k < ( counter - 1 ); k++ )
			}
			else // every other record
			{
				for(k = 0; k < counter; k++ )
				{
					if( k == j ); // do nothing
					else if( k == ( counter - 1 ) )
						fprintf( bPtrW, "%13.0lf:%s:%.2lf:%d:%d\0", book[k].bISBN, book[k].bTitle, book[k].bPrice, book[k].bQ, book[k].bDis );
					else
						fprintf( bPtrW, "%13.0lf:%s:%.2lf:%d:%d\n", book[k].bISBN, book[k].bTitle, book[k].bPrice, book[k].bQ, book[k].bDis );
				} // end for ( k = 0; k < counter; k++ )
			} // end if-else (j)
			fclose( bPtrW );
			printf( "\nYou have successfully deleted the book with the ISBN %.0lf.\n", book[j].bISBN ); // notification
			counter -= 1;
		} // end if
		else if( uConfirm == 'N' || uConfirm == 'n' ) // delete aborted
			return 0;
		else // invalid input
			printf( "Error.\n" );
	} // end if-else ( bnf == counter )
	return 0;
} // end <Delete book>

// <Show inventory> function
void opt4_showinventory( void )
{
   Book book[ counter ];
   int i = 0;
   FILE *bPtrR; // file pointer (read bookstore.txt)

   bPtrR = fopen( "bookstore.txt", "r" );

   // display <Show inventory> interface
   printf( "\n\nAll items currently available:\n" );
   printf( "ISBN            Book Name                      Price         Quantity      Discount(%%)\n" );
   printf( "**************************************************************************************\n" );

   while( !feof( bPtrR ) )
   {
      fscanf( bPtrR, "%lf:%[^:]:%lf:%d:%d\n", &book[i].bISBN, book[i].bTitle, &book[i].bPrice, &book[i].bQ, &book[i].bDis ); // reading items from file
      printf( "%.0lf   %-30s RM %6.2lf %9d %13d\n", book[i].bISBN, book[i].bTitle, book[i].bPrice, book[i].bQ, book[i].bDis ); // displaying items
      i++;
   } // end while

   printf( "\n" );

   // notfies user if there are books low on stock (book quantity < LOWSTOCK)
   for( i = 0; i < counter; i++ )
      if( book[i].bQ < LOWSTOCK )
         printf( "The book with ISBN %.0lf and title \"%s\" is running low on stock.\n", book[i].bISBN, book[i].bTitle );

   fclose( bPtrR );
} // end <Show inventory>

/*-------------------------------------------------------------------
/* END OF PROGRAM
/*-------------------------------------------------------------------
*/