/*-------------------------------------------------------------------
*/
/* ITS60304– Assignment #1 */
/* C Programming */
/* Student Name: <Tan Vi Lynn> <Heng Hian Wee> */
/* Student ID: <0320951> <0321343> */
/*-------------------------------------------------------------------
*/
  
#include <stdio.h>
 
// all function prototypes
int opt1_purchase( int dailyTrans[], int counter, FILE *fpB);
int opt2_update( int chosen);
int opt3_delete( int chosen ); 
int opt4_showinventory( FILE *fpB );  

//main function
int main( void )
{
  char temp;
  double tRev=0, dis=0;
  int dailyTrans[3]={0};//dailyTrans[0]-total books sold, dailyTrans[1]-total revenue, dailyTrans[2]-discount, array initialized to 0
  int menu_opt, counter=1; //menu option
  FILE *fpB;//file pointer for Bookstore.txt
  
  fpB = fopen("Bookstore.txt", "r");//open Bookstore.txt to read 

  if(fpB!=NULL)//check for existence of file
    while(!feof(fpB)) {
      temp=fgetc(fpB);
      if(temp=='\n')
		counter++;//Count the number of different types of books available
    }
  else
    printf("File not found.");

  rewind(fpB);//sets the pointer back to the first line
 
  // main menu interface
  do
  {
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
    scanf( "%d", &menu_opt ); //retrieve user selected option

    switch( menu_opt )
    {
      case 1: // <Purchase book> selected
        opt1_purchase(dailyTrans, counter, fpB);
        break;
 
      case 2: // <Update book> selected
        opt2_update( menu_opt );
        break;
 
      case 3: // <Delete book> selected
        opt3_delete( menu_opt );
        break;
 
      case 4: // <Show inventory> selected
        opt4_showinventory(fpB);
        break;
 
      case 5: // <Show daily transactions> selected
          tRev = (double)dailyTrans[1]/1000;
          dis = (double)dailyTrans[2]/1000;

          printf("\n\nDaily Transactions");
          printf("\n------------------------------------");
          printf( "\nTotal books sold         : %d", dailyTrans[0]);
          printf( "\nTotal revenue            : RM %.2lf", tRev);
          printf( "\nDiscount                 : RM %.2lf\n\n", dis);
        break;
 
      default: // other option(not 6) selected
      {
        if(menu_opt<1 || menu_opt>6)
          printf( "Invalid option. Please try again.\n" );
        else
          break;
      }
    }
  }
  while( menu_opt != 6 );// if menu_opt == 6, program terminate

  // notifies user that program termination is successful
  printf( "Program status: Terminated.\n" );
  printf( "Thank you for using this program. :)\n" );

  fclose(fpB);//close file - Bookstore.txt

  return 0;
 
} // end main
 
// <Purchase book> option
int opt1_purchase( int dailyTrans[], int counter, FILE *fpB)
{
  char bTitle[counter][100];//book title
  double uISBN, bISBN[counter], bPrice[counter];//user input ISBN, book ISBN, book price
  int i=0, bnf=0, uQ, bQ[counter], bDis[counter]; //bnf-book not found, user input quantity, book quantity, book discount
  double tPrice = 0;//total price = 0
  double tDis = 0;//total discount =0
 
  while( !feof( fpB ) )
  {
    fscanf( fpB, "%lf:%[^:]:%lf:%d:%d\n", &bISBN[i], bTitle[i], &bPrice[i], &bQ[i], &bDis[i]);//retrieve and store books in arrays
    i++;
  }

  rewind(fpB);//sets the pointer back to the first line

  printf( "\nEnter the 13-digit ISBN(Enter '-1' to indicate there is no book to be purchased): " );
  scanf( "%13lf", &uISBN ); // retrieve 13 ISBN digits

    
  if(uISBN==-1)//check if there is no book to be purchased
    return 0;
  else
  {
    printf("Enter the quantity per book: ");
    scanf("%d", &uQ);//retrieve quantity

    for(i=0;i<counter;i++)
    {
      if(uISBN==bISBN[i])
      {
        if(uQ>bQ[i])//check if user requested quantity is more than stock
        {
          printf("\nThe amount of books you requested is currently not available.\n");
          break;
        }
        else
        {
          tDis = (double)bDis[i]/100 * bPrice[i] * uQ;
          tPrice = (double)bPrice[i] * uQ - tDis;

          printf("\n\n***************RECEIPT****************\n\n");
          printf("ISBN:                      %13.0lf\n", bISBN[i]);
          printf("Book title:                %s\n", bTitle[i]);
          printf("Book Price:                RM %.2lf\n", bPrice[i]);
          printf("Quantity:                  %d\n", uQ);
          printf("TOTAL(after discount):     RM %.2lf(-%.2lf)", tPrice, tDis);
          printf("\n\n***************RECEIPT**************\n\n");

          dailyTrans[0] += uQ;
          dailyTrans[1] += (tPrice*1000);//increase accuracy of tPrice
          dailyTrans[2] += (tDis*1000);//increase accuracy of tDis
        }
      }
      else
      {
        bnf++;//if(uISBN!=bISBN[i]), then add 1 to book not found
      }
    }
  }//end for loop

  if(bnf==counter)//all books are scanned but book not found 
    printf("\nInvalid item.\n");

  return (int)dailyTrans;
} // end <Purchase book> function
  
// <Update book> option
int opt2_update( int chosen )
{
  printf( "\nThis option allows user to edit book.\n" );
 
  return chosen;
} // end <Update book> function

// <Delete book> option
int opt3_delete( int chosen )
{
  printf( "\nThis option allows user to delete book.\n" );
 
  return chosen;
} // end <Delete book> function

// <Show inventory> option
int opt4_showinventory(FILE *fpB)
{
  double isbn;
  char bookName[ 100 ];
  int quantity;
  double bookPrice;
  int discount;
 
  // Displaying <Show inventory> interface
  printf( "\n\nAll items currently available:\n" );
  printf( "ISBN          Book Name                      Price        Quantity      Discount Rate\n");
  printf( "**************************************************************************************\n");
 
 
  while( !feof( fpB ) )
  {
    fscanf( fpB, "%lf:%[^:]:%lf:%d:%d", &isbn, bookName, &bookPrice, &quantity, &discount ); // reading items from file
    printf( "%.0lf %-30s RM %6.2lf %5d %13d%%\n", isbn, bookName, bookPrice, quantity, discount ); // displaying items
  }
 
  return 0;
} // end <Show inventory> option