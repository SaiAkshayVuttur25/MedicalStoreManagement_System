/*
 1. ASSUMING EVERY MEDICATION HAS A DIFFERENT ID. EVEN IF THEY HAVE SAME BATCH NAME AND MEDICATION NAME
 2. IF YOUR NAMES HAVE A SPACE THEN REPLACE IT BY UNDERSCORES FOR SMOOTH FUNCTIONING
 3.YOU CAN ONLY ADD THE DATA INTO FILE AND STORE THE DATA INTO MY MEDICATION RECORDS FROM TEXT BCZ UPDATING , SORTING ALL THESE REQUIRE USAGE OF MANY TEMP FILES
 4. taking different reorder levels for different medicines because some medicines gets out of stock easily i.e bcz of demand for different diseases types...
5. ASSUMING MEDICATION AND SUPPLIER CANT HAVE ID 0
*/

//======================================================================================================================================================================
//HEADER FILES
#include <stdio.h>
#include <string.h> 
#include <stdbool.h>
#include <stdlib.h>  

#define MAX_SIZE 200
#define NAME_SIZE 100
#define BATCH_SIZE 50
#define EXP_DATE 9
#define CONTACT_SIZE 10
#define MAX_SUPPLIERS 20

//======================================================================================================================================================================

struct Supplier_Info{
    unsigned long Supplier_ID;
    char Supplier_Name[NAME_SIZE];
    unsigned int Quantity_of_stock_bysupplier;
    char Contact[CONTACT_SIZE];
};

struct ExpiryDate{
    int day;
    int month;
    int year;
};

typedef struct
{
    unsigned long Medication_ID;
    char Medicine_Name[NAME_SIZE];
    unsigned int Quantity_in_stock;
    unsigned int Price_per_Unit;
    int Reorderlevel;

    struct{
        char Batch[BATCH_SIZE];
        struct ExpiryDate Expiration_Date[1];
        int Total_sales;
    } Batch_details;

    struct Supplier_Info Suppliers[MAX_SUPPLIERS];

} Medications;

//======================================================================================================================================================================
//ALL GLOBAL VARIABLES
Medications medications[MAX_SIZE];
int total = sizeof(medications) / sizeof(Medications);
int current_level = 0; //THE NO OF RECORDS WHICH ARE FILLED
int got = 0; //TEMP VARIABLE TO USE INSTEAD OF BREAK STATEMENTS
int days_in_month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; //FOR EXPIRY DATE CALCULATION
int currsupp = 0; //AT EACH CASE I WANT TO STORE NO. OF SUPPLIERS IN DATABASE FOR EACH OF THE RECORD

//======================================================================================================================================================================

void Printing(){
    for (int i = 0; i < current_level; i++)
    {
        printf("\nMedication ID: %lu\n", medications[i].Medication_ID);
        printf("Medication Name: %s\n", medications[i].Medicine_Name);
        printf("Quantity in Stock: %u\n", medications[i].Quantity_in_stock);
        printf("Price per Unit: %u\n", medications[i].Price_per_Unit);
        printf("Reorder Level: %u\n", medications[i].Reorderlevel);
        printf("Batch: %s\n", medications[i].Batch_details.Batch);
        printf("Expiration Date: %d-%d-%d\n", medications[i].Batch_details.Expiration_Date[0].day, medications[i].Batch_details.Expiration_Date[0].month, medications[i].Batch_details.Expiration_Date[0].year);
        for(int j = 0; medications[i].Suppliers[j].Supplier_ID != 0 ;j++){
            printf("Supplier-%d ID : %lu\n",j+1,medications[i].Suppliers[j].Supplier_ID);
            printf("Supplier-%d Name : %s\n",j+1,medications[i].Suppliers[j].Supplier_Name);
            printf("Supplier-%d Quantity_Provided : %d\n",j+1,medications[i].Suppliers[j].Quantity_of_stock_bysupplier);
            printf("Supplier-%d Contact Number : %s\n",j+1,medications[i].Suppliers[j].Contact);
        }
        printf("Total Sales is %d\n",medications[i].Batch_details.Total_sales);
    }

}

//======================================================================================================================================================================

void Initialise_data(Medications *medications, int st, int size){

    for (int i = st; i < size; i++)
    {
        medications[i].Medication_ID = 0;
        medications[i].Medicine_Name[0] = '\0';
        medications[i].Quantity_in_stock = 0;
        medications[i].Price_per_Unit = 0;
        medications[i].Reorderlevel = 0;
        medications[i].Batch_details.Batch[0] = '\0';
        for (int k = 0; k < 3; k++) {
            medications[i].Batch_details.Expiration_Date[k].day = 0;
            medications[i].Batch_details.Expiration_Date[k].month = 0;
            medications[i].Batch_details.Expiration_Date[k].year = 0;
        }
        medications[i].Batch_details.Total_sales = 0;
        for(int j=0;j<MAX_SUPPLIERS;j++){
            medications[i].Suppliers[j].Supplier_ID = 0;
            medications[i].Suppliers[j].Supplier_Name[0] = '\0';
            medications[i].Suppliers[j].Quantity_of_stock_bysupplier = 0;
            medications[i].Suppliers[j].Contact[CONTACT_SIZE] = '\0';
        }
    }
}

//======================================================================================================================================================================

int CheckMEdicIDExist(unsigned long newID){
    for(int i=0;i<current_level;i++){
        if(medications[i].Medication_ID==newID){
            printf("There Exist the record with same ID You Entered Already. Please Enter Another ID! : ");
            return 0;
        }
    }
    return 1;
}

int checkSuppID(unsigned long SupID , int i){
    for(int j=0;j<currsupp;j++){
        if(medications[i].Suppliers[j].Supplier_ID==SupID){
            printf("There Exist the record with same SUPP ID You Entered Already. Please Enter Another ID! : ");
            return 0;
        }
    }
    return 1;
}
void Add_New_Medication(Medications *medications, int i){
    
    printf("Enter Medication ID: ");
    unsigned long newID;
    scanf("%lu", &newID);

    while(!CheckMEdicIDExist(newID)) {
        scanf("%lu", &newID); 
    }
    medications[i].Medication_ID = newID; 

    printf("Enter Medication Name: ");
    scanf("%s", medications[i].Medicine_Name);

    printf("Enter Price per Unit: ");
    scanf("%d", &medications[i].Price_per_Unit);

    printf("Enter the Batch Name: ");
    scanf("%s", medications[i].Batch_details.Batch);

    int day, month, year;
    printf("Enter Expiration Date in DD-MM-YYYY \n");
    printf("Enter Day :");
    scanf("%d", &day);
    printf("Enter Month :");
    scanf("%d", &month);
    printf("Enter Year :");
    scanf("%d", &year);
    
    medications[i].Batch_details.Expiration_Date[0].day = day;
    medications[i].Batch_details.Expiration_Date[0].month = month;
    medications[i].Batch_details.Expiration_Date[0].year = year;

    int suppliers;
    printf("\nEnter the No. of Suppliers : ");
    scanf("%d", &suppliers);
    currsupp = suppliers;

    for(int j=0;j<suppliers;j++){

        unsigned long SupID;
        printf("Enter Supplier-%d ID : ",j+1);
        scanf("%lu", &SupID);

        
        while(!checkSuppID(SupID, i)) {
            scanf("%lu", &SupID); 
        }

        medications[i].Suppliers[j].Supplier_ID = SupID;
        printf("Enter Supplier-%d Name : ",j+1);
        scanf("%s", medications[i].Suppliers[j].Supplier_Name);

        printf("Enter Quantity of medication by supplier-%d : ",j+1);
        scanf("%d", &medications[i].Suppliers[j].Quantity_of_stock_bysupplier);

        medications[i].Quantity_in_stock += medications[i].Suppliers[j].Quantity_of_stock_bysupplier;
        
        printf("Enter Contact details for Supplier-%d (10 digits) : ",j+1);
        scanf("%s", &medications[i].Suppliers[j].Contact);
        printf("\n");

    }

    printf("Enter the Reorder level (between 10-100): ");
    scanf("%d", &medications[i].Reorderlevel); 

    // if user didnot enter the reorder level in the given range asking him till he enter correct reorder level
   
    while(medications[i].Reorderlevel < 10 || medications[i].Reorderlevel >100){
        printf("Please Enter Valid Reorder Level in the range (10-100) ");
        scanf("%d", &medications[i].Reorderlevel);
    }
}

//======================================================================================================================================================================

void Update_Details(Medications *medications ,int key,int id,int total){

    int i, found = 0;
    //Checking Whether the given id exits in the medication record or not and also finding the row where this id exist ans storing in i
    for(i = 0; medications[i].Medication_ID!=0 && !found; i++){
        if(medications[i].Medication_ID == id ){
            found = 1;
            i--; // Step back to correct index since loop increments i
        }
    }

    if(found == 0){
        printf("Medication not found\n");
    }
    else {
        switch(key){
            case 1:
                //Am assuming that by somehow data got changed so to correct it we are modifing quantity.
                //if not we should ask which users had enter wrong info which is complex
                printf("Enter Quantity of medication: ");
                scanf("%d", &medications[i].Quantity_in_stock );
                break;
            
            case 2: 
                printf("Enter Price Per Unit: ");
                scanf("%d", &medications[i].Price_per_Unit );
                break;
            
            case 3:
                printf("Enter Reorder Level: ");
                scanf("%d", &medications[i].Reorderlevel );
                break;

            case 4:
                printf("Enter Batch Name: ");
                // As taking for loop and instead of replacing every characcter with other i am storing input in temp1 and copying back to record again
                char temp1[BATCH_SIZE];
                scanf("%s", temp1);
                strcpy(medications[i].Batch_details.Batch, temp1);
                break;

            case 5:
                printf("Enter Expiration Date (DD-MM-YYYY): ");
                int day, month, year;
                scanf("%d",&day);
                scanf("%d",&month);
                scanf("%d",&year);
                
                medications[i].Batch_details.Expiration_Date[0].day = day;
                medications[i].Batch_details.Expiration_Date[0].month = month;
                medications[i].Batch_details.Expiration_Date[0].year = year;
                break;
            
            default:
                printf("Invalid option.\n");
                break;    
        }
    }
}


void Update_Medication_Details(Medications *medications,int total){

    int flag=1;
    while(flag){ 
        int req ;
        unsigned long DetailsID_ToUpdate;

        printf("\n1.Quantity_in_stock\n2.Price_per_unit\n3.Reorder_level\n4.Batch_name\n5.Expiration_date\n0.Exit\n");

        printf("\nEnter the key for details You Want to Modify or (0 to exit):");
        scanf("%d", &req);

        if(req!=0){
            printf("\nEnter the ID of Medication You want to Update :");
            scanf("%lu", &DetailsID_ToUpdate);
        }
        if(req==0){
            flag=0;
        }
        else if(req>5 || req<0){
            printf("Invalid Key.Enter a New Key");
        }
        else{
            Update_Details(medications , req , DetailsID_ToUpdate,total);

        }
    }
}

//======================================================================================================================================================================

void Delete_Medication(Medications *medications, int total, int id, char batchNo[])
{
    // removing the medication record based on batchNo from the medications
    if (id == -1)
    { // remove particular batch records
        got = 0;
        for (int i = 0; medications[i].Medication_ID!=0 ; i++) {
            if (strcmp(medications[i].Batch_details.Batch, batchNo) == 0)
            {
                int j;
                got =1;
                for (j = i; medications[j].Medication_ID!=0; j++)
                {
                    medications[j] = medications[j + 1];
                }
                Initialise_data(medications,j-1,j);
                i--; // decrement index because we are removing one element as we should delete batch and there might be multiple batches
            }
        }
        if(got == 0){
            printf("Enter the Valid BatchName ! or Batch Name Doesnt Exist !\n");
        }
    }
    else {
        int found = 0;  // Use a flag to track if the medication was found
        for (int i = 0; medications[i].Medication_ID!=0 && !found; i++) {
            if (medications[i].Medication_ID == id) {
                // Shift elements to the left after removing
                int j;
                for (j = i; medications[j].Medication_ID!=0 ; j++) {
                    medications[j] = medications[j + 1];
                }
                Initialise_data(medications, j-1, j);
                found = 1;  // Marking as found no need of decrement as there exist unique id
            }
        }
        if(found==0){
            printf("Enter the Valid BatchName ! or Batch Name Doesnt Exist !\n");
        }
    }
}

//======================================================================================================================================================================

void Search_RelevantDetails(Medications *medications, int key, int i) {
    switch (key) {
        case 1:
            printf("Quantity of medication : %d \n", medications[i].Quantity_in_stock);
            break;

        case 2:
            printf("Price Per Unit of Medication : %d \n", medications[i].Price_per_Unit);
            break;

        case 3:
            printf("Reorder Level of Medication : %d \n", medications[i].Reorderlevel);
            break;

        case 4:
            printf("Batch Name : %s \n", medications[i].Batch_details.Batch);
            break;

        case 5:
            printf("Expiration Date: %02d/%02d/%04d\n",
                   medications[i].Batch_details.Expiration_Date[0].day,
                   medications[i].Batch_details.Expiration_Date[0].month,
                   medications[i].Batch_details.Expiration_Date[0].year);
            break;

        case 6:
            for (int j = 0; medications[i].Suppliers[j].Supplier_ID != 0 && j<total; j++) {
                printf("Supplier%d Supplier_ID : %lu \n", j + 1, medications[i].Suppliers[j].Supplier_ID);
            }
            
            break;

        case 7:
            for (int j = 0; medications[i].Suppliers[j].Supplier_ID != 0; j++) {
                printf("Supplier%d Supplier Name : %s \n", j + 1, medications[i].Suppliers[j].Supplier_Name);
            }
            break;

        case 8:
            for (int j = 0; medications[i].Suppliers[j].Supplier_ID != 0; j++) {
                printf("Supplier%d Supplied Quantity: %d \n", j + 1, medications[i].Suppliers[j].Quantity_of_stock_bysupplier);
            }
            break;

        case 9:
            for (int j = 0; medications[i].Suppliers[j].Supplier_ID != 0; j++) {
                printf("Supplier%d Contact details : %s \n", j + 1, medications[i].Suppliers[j].Contact);
            }
            break;
        case 10 :
            printf("Medication ID is  %lu ",medications[i].Medication_ID);
            break;

        default:
            printf("Invalid option.\n");
            break;
    }
}

// Function to search for medication using ID, batch name, or supplier ID
void Search_Medication(Medications *medications, int total, unsigned long id, char *medicineName, unsigned long supplierID) {

    int got = 0;  // To check if medication is found or not
    int flag = 1;  // To control the while loop
    int temp = 0;  // temp variable to stop searching when conditn fullfilled

    // Search by Medication ID
    if (id != 0) {
        for (int i = 0; i < total && flag; i++) {
            if (medications[i].Medication_ID == id) {
                got = 1;
                while (flag) {
                    int key;
                    printf("\n1.Quantity_in_stock\n2.Price_per_unit\n3.Reorder_level\n4.Batch_name\n5.Expiration_date\n6.Supplier_id\n7.Supplier_name\n8.Quantity_supplied_by_supplier\n9.ContactDetails\n10.Medication_ID\n0.Exit\n");
                    printf("Enter The Key : ");
                    scanf("%d", &key);
                    if (key == 0) {
                        flag = 0;
                    } else {
                        Search_RelevantDetails(medications, key, i);
                    }
                }
                temp = 1;
            }
        }
    }
    // Search by Medicine Name
    else if (medicineName != NULL) {
        for (int i = 0; i < total && flag; i++) {
            if (strcmp(medications[i].Medicine_Name, medicineName) == 0) {
                got = 1;
                while (flag) {
                    int key;
                    printf("\n1.Quantity_in_stock\n2.Price_per_unit\n3.Reorder_level\n4.Batch_name\n5.Expiration_date\n6.Supplier_id\n7.Supplier_name\n8.Quantity_supplied_by_supplier\n9.ContactDetails\n10.Medication_ID\n0.Exit\n");
                    printf("Enter The Key : ");
                    scanf("%d", &key);
                    if (key == 0) {
                        flag = 0;
                    } else {
                        Search_RelevantDetails(medications, key, i);
                    }
                }
                temp = 1;
            }
        }
    }
    // Search by Supplier ID
    else if (supplierID != 0) {
        for (int i = 0; medications[i].Medication_ID != 0; i++) {
            for (int j = 0; j < MAX_SUPPLIERS; j++) {
                if (medications[i].Suppliers[j].Supplier_ID == supplierID) {
                    
                    got = 1;// Supplier found, show the medication details
                    temp = 1; // Mark that we found a supplier match

                    printf("\nMedication ID is  %lu \n",medications[i].Medication_ID);
                    printf("Medication Name is  %s \n",medications[i].Medicine_Name);
                }
            }
        }
    }

    if (!got) {
        printf("No Medicine found with the given details.\n");
        
    }
}
//======================================================================================================================================================================

void Generate_Alerts(Medications *medications,int total){
    for(int i=0;medications[i].Medication_ID != 0;i++){
        if (medications[i].Quantity_in_stock < medications[i].Reorderlevel){
            printf("\n==================== ALERT ====================\n");
            printf("Medication ID: %lu\n", medications[i].Medication_ID);
            printf("Batch Name: %s\n", medications[i].Batch_details.Batch);
            printf("Status: Insufficient stock!\n");
            printf("Current Quantity: %d\n", medications[i].Quantity_in_stock);
            printf("===============================================\n");
        }
    }
}

//======================================================================================================================================================================

int Check_leap_year(int year) {
    //FINDING GIVEN YEAR IS LEAP YEAR OR NOT
    if((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)){
        return 1;
    }
    return 0;
}

int DaysCompleted_InYear(int day, int month, int year) {
    int total_days = 0;
    //Till before month we are adding all days as we have an array storing days of 12 months
    for (int i = 1; i < month; i++) {
        total_days += days_in_month[i - 1];
        if (i == 2 && Check_leap_year(year)) {
            total_days += 1;
        }
    }
    total_days += day;
    return total_days;
}

int date_difference(int day1, int month1, int year1, int day2, int month2, int year2) {
    int days_Complemented_year1 = DaysCompleted_InYear(day1, month1, year1);
    int days_Complemented_year2 = DaysCompleted_InYear(day2, month2, year2);

    if (year1 == year2) {
        return days_Complemented_year2 - days_Complemented_year1;
    } 
    else {
        int days_remaining_in_year1;
        if(Check_leap_year(year1)){
            days_remaining_in_year1 =  366  - days_Complemented_year1;
        }
        else{
            days_remaining_in_year1 =  365  - days_Complemented_year1;
        }
        
        int days_bw_years = 0;
        for (int i = year1 + 1; i < year2; i++) {
            days_bw_years += Check_leap_year(i) ? 366 : 365;
        }

        return days_remaining_in_year1 + days_Complemented_year2 + days_bw_years;
    }
}

//For Same batch as there is same expiration Date so Sendling Alerts of All medications individually
void Check_Expiration_Dates(Medications *medications, int total , int day , int month ,int year){
    for(int i=0;medications[i].Medication_ID!=0;i++){
        //expiration dates
        int eday = medications[i].Batch_details.Expiration_Date[0].day;
        int emonth = medications[i].Batch_details.Expiration_Date[0].month;
        int eyear = medications[i].Batch_details.Expiration_Date[0].year;

        int result = date_difference(day,month,year,eday,emonth,eyear);

        if(result<=30){
            printf("\n==================== ALERT ====================\n");
            printf("Medication ID: %lu\n", medications[i].Medication_ID);
            printf("Batch Name: %s\n", medications[i].Batch_details.Batch);
            printf("Expiration Date: %02d/%02d/%04d\n", 
                medications[i].Batch_details.Expiration_Date[0].day,
                medications[i].Batch_details.Expiration_Date[0].month,
                medications[i].Batch_details.Expiration_Date[0].year);
            if(result<=0){
                printf("Medication Already Expired\n");
            }
            else{
                printf("%d Remaining Days Left to Expire\n",result);
            }
            printf("===============================================\n");

        }
    }
    printf("----------------------------------------------------------------------------------------------------------------------------\n");
}

//======================================================================================================================================================================

// Even though insertion sort is best in this case still bcz once we sorted even in worst case then after next add medications only 1 elmt is to be inserted make it efficient but my code dont want to sort the all medications in my file only sort it in main or output file.. may be our medicines were in batchwise sorted so i dont want to disturb them..

void swap(Medications *a, Medications *b) {
    Medications temp = *a;
    *a = *b;
    *b = temp;
}


int partition(Medications *medications, int low, int high) {
    //assuming last elmt as pivot
    Medications pivot = medications[high]; 

    int PivotYr = pivot.Batch_details.Expiration_Date[0].year;
    int PivotMonth = pivot.Batch_details.Expiration_Date[0].month;
    int PivotDay = pivot.Batch_details.Expiration_Date[0].day;

    int YearCheck;
    int MonthCheck;
    int DayCheck;

    int i = low - 1; 
    for(int j = low; j < high; j++) {

        //First checking Years if they are equal then go to month if that too equal then go to day to sort
        YearCheck = medications[j].Batch_details.Expiration_Date[0].year;
        MonthCheck = medications[j].Batch_details.Expiration_Date[0].month;
        DayCheck = medications[j].Batch_details.Expiration_Date[0].day;

        if ((YearCheck < PivotYr) || ((YearCheck== PivotYr) && (MonthCheck < PivotMonth)) ||(((YearCheck == PivotYr) && (MonthCheck == PivotMonth)) &&( DayCheck < PivotDay))) {
            i++; 
            swap(&medications[i], &medications[j]);
        }
    }

    swap(&medications[i + 1], &medications[high]); 
    return (i + 1);  
}

void quicksort(Medications *medications, int low, int high) {
    if (low < high) {
        int partInd = partition(medications, low, high);  
        quicksort(medications, low, partInd - 1);
        quicksort(medications, partInd + 1, high);
    }
}

void Sorting_ExpDate() {
    int sz = 0;
    // Calculating the size of the array (assuming Medication_ID = 0 indicates end of array)
    for(int i = 0; medications[i].Medication_ID != 0; i++) {
        sz++;
    }

    quicksort(medications, 0, sz - 1);  
}

//======================================================================================================================================================================

void salesTracking(Medications *medications , int total, char BatchName[]){
    int TotSales = 0,QuantitySold=0;
    printf("Enter the Total Sales of Batch %s -: ",BatchName);
    scanf("%d", &TotSales);
    got = 0;
    
    for(int i=0;medications[i].Medication_ID!=0;i++){
        if(strcmp(medications[i].Batch_details.Batch, BatchName)==0){
            got =1;
            printf("Enter the Quantity Sold from the Batch with this ID %lu - : ",medications[i].Medication_ID);
            scanf("%d",&QuantitySold);
            medications[i].Quantity_in_stock -= QuantitySold;
            TotSales+=QuantitySold;
        }
    }
    if(got==0){
        printf("There Is no Medication With That Batch Name ! \n");
    }
    else{
        for(int i=0;i<medications[i].Medication_ID!=0;i++){
            if(strcmp(medications[i].Batch_details.Batch, BatchName)==0){
                medications[i].Batch_details.Total_sales = QuantitySold;
            }
        }
    }
}

//======================================================================================================================================================================

void SupplierManagement(Medications *medications , int total){
    int key_Supplier;
    printf("\n1.For Adding A supplier.\n2.Update Supplier Details\n3.Search for Suplier\n4.Associate Suppliers with their medications\n");
    scanf("%d",&key_Supplier);
    switch (key_Supplier)
    {
        case 1:
            {
            unsigned long id_forManagwe;
            printf("Enter id of the Medication : ");
            scanf("%d",&id_forManagwe);
            got=1;
            for(int i=0;i<total && got;i++){
                if(medications[i].Medication_ID == id_forManagwe){
                    int j;
                    for(j=0;j<total && medications[i].Suppliers[j].Supplier_ID !=0;j++){
                        j++;
                    }
                    printf("\nEnter No. of Suppliers : ");
                    int ReqSupp;
                    scanf("%d",&ReqSupp);
                    ReqSupp+=j;
                    for(;j<ReqSupp;j++) {
                        printf("Enter the Supplier Details \n");
                        printf("Enter the New SuplierID : ");
                        unsigned long supid;
                        scanf("%lu",&supid);
                        int found = 0;
                        int k = 0 ;
                        for(k=0;k<j && !found ;k++){
                            if(medications[i].Suppliers[k].Supplier_ID == supid){
                                printf("The Supplier Already Exist Enter New One or u can modify details : ");
                                scanf("%lu",&supid);
                                found = 1;
                            }
                        }
                        medications[i].Suppliers[j].Supplier_ID = supid;
                        printf("\n");
                        printf("\nEnter the New SuplierName : ");
                        scanf("%s",&medications[i].Suppliers[j].Supplier_Name);
                        printf("\n");
                        printf("\nEnter the Quantity Provided by Supplier : ");
                        scanf("%d",&medications[i].Suppliers[j].Quantity_of_stock_bysupplier);
                        printf("\n");
                        medications[i].Quantity_in_stock += medications[i].Suppliers[j].Quantity_of_stock_bysupplier;
                        printf("\nEnter the ContactName : ");
                        scanf("%s",&medications[i].Suppliers[j].Contact);
                        printf("\n");
                    }
                }
            } 
            break;}
        case 2:
            {unsigned long UpdtSupplierId;

            printf("Enter Supplier ID");
            scanf("%lu",&UpdtSupplierId);
            for(int i=0;medications[i].Medication_ID !=0 ;i++){
                got=1;
                for(int j=0;medications[i].Suppliers[j].Supplier_ID!=0 && got;j++){
                    if(medications[i].Suppliers[j].Supplier_ID == UpdtSupplierId){
                        printf("Enter the New Supplier Details \n");
                        printf("Enter Updated Supplier Name : ");
                        scanf("%s",&medications[i].Suppliers[j].Supplier_Name);
                        printf("\n");
                        printf("Enter Updated Supplier Stock supplied : ");
                        medications[i].Quantity_in_stock-=medications[i].Suppliers[j].Quantity_of_stock_bysupplier;
                        scanf("%d",&medications[i].Suppliers[j].Quantity_of_stock_bysupplier);
                        medications[i].Quantity_in_stock+=medications[i].Suppliers[j].Quantity_of_stock_bysupplier;
                        printf("\n");
                        printf("Enter Updated Supplier Contact : ");
                        scanf("%s",&medications[i].Suppliers[j].Contact);
                        printf("\n");
                        got = 0;
                    }
                } 
            }
            break;}

        case 3:
            {unsigned long SearchSupplierId;
            printf("Enter Supplier ID : ");
            scanf("%lu",&SearchSupplierId);
            Search_Medication(medications,total,0,NULL,SearchSupplierId);
            break;}

        case 4:
            {unsigned long SupplierId;
            printf("Enter Supplier ID : ");
            scanf("%lu",&SupplierId);
            got = 0;
            for (int i = 0; medications[i].Medication_ID != 0; i++) {
                for (int j = 0; j < MAX_SUPPLIERS; j++) {
                    if (medications[i].Suppliers[j].Supplier_ID == SupplierId) {
                        got = 1;// Supplier found, show the medication details
                        printf("\nMedication ID is  %lu \n",medications[i].Medication_ID);
                        printf("Medication Name is  %s \n",medications[i].Medicine_Name);
                    }
                }
            }
            if(got==0){
                printf("Supplier with the Given SupplierId is not found");
            }
            break;}

        default:
            break;
    }

}

//======================================================================================================================================================================

typedef struct {
    unsigned long Supplier_ID;
    char Supplier_Name[100];
    int unique_med_count;
    unsigned long total_cost;
} TopSuppliers;

void swapTopSuppliers(TopSuppliers *a, TopSuppliers *b) {
    TopSuppliers temp = *a;
    *a = *b;
    *b = temp;
}

int find_supplier(TopSuppliers *Details, int size, unsigned long supplier_id) {
    for (int i = 0; i < size; i++) {
        if (Details[i].Supplier_ID == supplier_id) {
            return i;  
        }
    }
    return -1; 
}

int partitionTopSuppliers(TopSuppliers *Details, int low, int high, int choice) {
    TopSuppliers pivot = Details[high];  // considering Last element as pivot
    int i = low - 1;  // starting from smallest element

    for (int j = low; j < high; j++) {
        // Sort in descending order of total_cost 
        if (choice) {
            if (Details[j].total_cost > pivot.total_cost) {
                i++;
                swapTopSuppliers(&Details[i], &Details[j]);
            }
        } 
        // Sort in descending order of unique_med_count
        else {
            if (Details[j].unique_med_count > pivot.unique_med_count) {
                i++;
                swapTopSuppliers(&Details[i], &Details[j]);
            }
        }
    }

    swapTopSuppliers(&Details[i + 1], &Details[high]);  
    return (i + 1);  
}

void quicksortTopSuppliers(TopSuppliers *Details, int low, int high, int choice) {
    if (low < high) {
        int partInd = partitionTopSuppliers(Details, low, high, choice);  // Get partition index
        quicksortTopSuppliers(Details, low, partInd - 1, choice);
        quicksortTopSuppliers(Details, partInd + 1, high, choice);
    }
}

void find_allrounder_suppliers(Medications *medications, int total) {
    TopSuppliers Details[MAX_SUPPLIERS]; 
    int DetailSize = 0;  
    for (int i = 0; medications[i].Medication_ID!=0; i++) {
        for (int j = 0; medications[i].Suppliers[j].Supplier_ID != 0; j++) {
            int index = find_supplier(Details, DetailSize, medications[i].Suppliers[j].Supplier_ID);
            if (index == -1) {
                Details[DetailSize].Supplier_ID = medications[i].Suppliers[j].Supplier_ID;
                strcpy(Details[DetailSize].Supplier_Name, medications[i].Suppliers[j].Supplier_Name);
                Details[DetailSize].unique_med_count = 1; 
                Details[DetailSize].total_cost = 0;  // Initializing total_cost to 0 
                DetailSize++;
            } else {
                Details[index].unique_med_count++;
            }
        }
    }

    // Sort suppliers based on unique_med_count in descending order
    quicksortTopSuppliers(Details, 0, DetailSize - 1, 0);

    // Printing top 10 all-rounder suppliers
    printf("Top 10 All-Rounder Suppliers (based on unique medications supplied):\n");
    for (int i = 0; i < DetailSize && i < 10; i++) {
        printf("%d. Supplier ID: %lu, Name: %s, Unique Medications: %d\n",
               i + 1, Details[i].Supplier_ID, Details[i].Supplier_Name, Details[i].unique_med_count);
    }
    printf("\n");
    printf("The Best Allrounder is %s (ID-%lu) with his unique medications count of %d \n",Details[0].Supplier_Name,Details[0].Supplier_ID,Details[0].unique_med_count);
}

void find_largest_turnover_suppliers(Medications medications[], int total) {
    TopSuppliers Details[MAX_SUPPLIERS] = {0};
    int supplierCount = 0;

    // Traverse all medications and suppliers to calculate total cost
    for (int i = 0; i < total; i++) {
        for (int j = 0; j < MAX_SUPPLIERS; j++) {
            if (medications[i].Suppliers[j].Supplier_ID != 0) {
                // Now Calculate total cost for this supplier: quantity x price per unit
                unsigned long cost = medications[i].Suppliers[j].Quantity_of_stock_bysupplier * medications[i].Price_per_Unit;

                // Check if this supplier is already in Details array
                int index = find_supplier(Details, supplierCount, medications[i].Suppliers[j].Supplier_ID);

                if (index == -1) {
                    Details[supplierCount].Supplier_ID = medications[i].Suppliers[j].Supplier_ID;
                    strcpy(Details[supplierCount].Supplier_Name, medications[i].Suppliers[j].Supplier_Name);
                    Details[supplierCount].total_cost = cost;
                    // Initialize unique_med_count as 0 for this scenario
                    Details[supplierCount].unique_med_count = 0;  
                    supplierCount++;
                } else {
                    Details[index].total_cost += cost;
                }
            }
        }
    }

    // Sort suppliers by total cost in descending order
    quicksortTopSuppliers(Details, 0, supplierCount - 1, 1);

    // Print top 10 suppliers with the highest total cost
    printf("Top 10 Suppliers by Total Cost:\n");
    for (int i = 0; i < 10 && i < supplierCount; i++) {
        printf("Supplier ID: %lu, Supplier Name: %s, Total Cost: %lu\n",
            Details[i].Supplier_ID, Details[i].Supplier_Name, Details[i].total_cost);
    }
    printf("\n");
    printf("The Supplier %s and ID %lu has Best TurnOver of  %lu \n",Details[0].Supplier_ID,Details[0].Supplier_Name,Details[0].total_cost);
}

//======================================================================================================================================================================

//FILEHANDLING
void ReadFileAndStoreData(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Unable to open file %s\n", filename);
        return;
    }

    char Buffer[256];
    int i = 0;  

    while (fgets(Buffer, sizeof(Buffer), file)) {
        // if the line is empty then u just continue the loop and Move to the beginning of the line and overwrite
        if (Buffer[0] == '\n' || Buffer[0] == '\r') {
            continue;
        }
        //reads formatted input from a string rather than standard input...at first i stored id quantity... etc. 
        sscanf(Buffer, "%lu", &medications[i].Medication_ID);
        fgetc(file);  // if the newline exist then consume it


        fgets(medications[i].Medicine_Name, sizeof(medications[i].Medicine_Name), file);
        // at first i added for proper readibility. in my textfile
        medications[i].Medicine_Name[strcspn(medications[i].Medicine_Name, "\n")] = 0;  // Remove newline

        fscanf(file, "%u", &medications[i].Quantity_in_stock);
        fscanf(file, "%u", &medications[i].Price_per_Unit);
        fscanf(file, "%s", medications[i].Batch_details.Batch);

        fscanf(file, "%d %d %d", 
            &medications[i].Batch_details.Expiration_Date[0].day,
            &medications[i].Batch_details.Expiration_Date[0].month,
            &medications[i].Batch_details.Expiration_Date[0].year
        );

        int num_suppliers;
        fscanf(file, "%d", &num_suppliers);

        for (int j = 0; j < num_suppliers; j++) {
            fscanf(file, "%lu", &medications[i].Suppliers[j].Supplier_ID);
            fscanf(file, "%s", medications[i].Suppliers[j].Supplier_Name);
            fscanf(file, "%u", &medications[i].Suppliers[j].Quantity_of_stock_bysupplier);
            fscanf(file, "%s", medications[i].Suppliers[j].Contact);
        }

        fscanf(file, "%d", &medications[i].Reorderlevel);

        i++; 
        current_level++; //as i want to store total data
    }

    fclose(file);
}

void AppendRecordToFile(const char* filename, int index) {
    FILE *file = fopen(filename, "a");
    if (!file) {
        printf("Unable to open file %s for appending\n", filename);
        return;
    }
    // if i enter wrong index
    if (index < 0 || index >= MAX_SIZE || medications[index].Medication_ID == 0) {
        printf("Invalid index for appending record.\n");
        fclose(file);
        return;
    }

    fprintf(file, "\n"); // Add a newline before entering the new record
    fprintf(file, "%lu\n", medications[index].Medication_ID);
    fprintf(file, "%s\n", medications[index].Medicine_Name);
    fprintf(file, "%u\n", medications[index].Quantity_in_stock);
    fprintf(file, "%u\n", medications[index].Price_per_Unit);
    fprintf(file, "%s\n", medications[index].Batch_details.Batch);
    fprintf(file, "%d\n", medications[index].Batch_details.Expiration_Date[0].day);
    fprintf(file, "%d\n", medications[index].Batch_details.Expiration_Date[0].month);
    fprintf(file, "%d\n", medications[index].Batch_details.Expiration_Date[0].year);
    fprintf(file, "%d\n", currsupp);
    for (int j = 0; j < MAX_SUPPLIERS && medications[index].Suppliers[j].Supplier_ID != 0; j++) {
        fprintf(file, "%lu\n", medications[index].Suppliers[j].Supplier_ID);
        fprintf(file, "%s\n", medications[index].Suppliers[j].Supplier_Name);
        fprintf(file, "%u\n", medications[index].Suppliers[j].Quantity_of_stock_bysupplier);
        fprintf(file, "%s\n", medications[index].Suppliers[j].Contact);
    }

    fprintf(file, "%d\n", medications[index].Reorderlevel);

    fclose(file);
    printf("\nRecord successfully appended to the file.\n");
}

void WriteAllDataToFile(const char* filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("\nUnable to open file %s for writing\n", filename);
        return;
    }

    for (int i = 0; i <= current_level; i++) {
        fprintf(file, "%lu\n", medications[i].Medication_ID);
        fprintf(file, "%s\n", medications[i].Medicine_Name);
        fprintf(file, "%u\n", medications[i].Quantity_in_stock);
        fprintf(file, "%u\n", medications[i].Price_per_Unit);
        fprintf(file, "%s\n", medications[i].Batch_details.Batch);
        fprintf(file, "%d\n", medications[i].Batch_details.Expiration_Date[0].day);
        fprintf(file, "%d\n", medications[i].Batch_details.Expiration_Date[0].month);
        fprintf(file, "%d\n", medications[i].Batch_details.Expiration_Date[0].year);

        int num_suppliers = 0;
        for (int j = 0; j < MAX_SUPPLIERS && medications[i].Suppliers[j].Supplier_ID != 0; j++) {
            num_suppliers++;
        }

        fprintf(file, "%d\n", num_suppliers);  // Write the number of suppliers

        for (int j = 0; j < num_suppliers; j++) {
            fprintf(file, "%lu\n", medications[i].Suppliers[j].Supplier_ID);
            fprintf(file, "%s\n", medications[i].Suppliers[j].Supplier_Name);
            fprintf(file, "%u\n", medications[i].Suppliers[j].Quantity_of_stock_bysupplier);
            fprintf(file, "%s\n", medications[i].Suppliers[j].Contact);
        }

        fprintf(file, "%d\n", medications[i].Reorderlevel);
        fprintf(file, "\n");
    }

    fclose(file);
    printf("\nAll medication records successfully written to the file '%s'.\n", filename);
}
//======================================================================================================================================================================

int main(){

    Initialise_data(medications, 0,total);

    ReadFileAndStoreData("medication.txt");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("  (\\   /)\n"); 
    printf("   ( . .)\n");   
    printf("    > ^ <\n");

    printf(" Welcome to the India's Top Medical Store. \n");
    int flag = 1;
    while (flag)
    { // Infinite loop to keep taking inputs until user exits
        printf("\n1. Add New Medication\n2. Update Medication Details\n3. Delete Medication\n4. Search Medication");
        printf("\n5. Stock Alerts\n6. Check Expiration Dates\n7. Sort Medication By Expiration Dates");
        printf("\n8. Sales Tracking\n9. Supplier Management\n10. Find All-rounder Suppliers\n11. Find Suppliers with Largest Turn-over\n12. Print the Whole Data\n0. Exit\n");

        int response;
        printf("\nEnter the Operation You want to monitor : ");
        scanf("%d", &response);

        switch (response)
        {
        case 0:
            flag = 0;
            printf("You are now exitted from the process. re run to start ur process again !!!");
            break;
            printf("----------------------------------------------------------------------------------------------------------------------------\n");


        case 1:
            Add_New_Medication(medications, current_level);

            int choice;
            printf("\nTo Add this record to 'medication.txt'? press 1\nTo Add this record to 'output.txt' press2\nPress 3 for both\nPress any number for none : ");
            scanf(" %d", &choice);

            if (choice == 1 ) {
                AppendRecordToFile("medication.txt", current_level);  
            }
            else if(choice==2){
                WriteAllDataToFile("output.txt");
            }
            else if(choice ==3){
                AppendRecordToFile("medication.txt", current_level);
                WriteAllDataToFile("output.txt");
            }

            current_level++;
            printf("----------------------------------------------------------------------------------------------------------------------------\n");


            break;

        case 2:
            // Add update medication logic
            Update_Medication_Details(medications,total);
            char choice2;
            printf("Do you want to change this record into 'output.txt'? (y/n): ");
            scanf(" %c", &choice2);

            if (choice2 == 'y' || choice2=='Y') {
                WriteAllDataToFile("output.txt");  
            }
            printf("----------------------------------------------------------------------------------------------------------------------------\n");

            break;

        case 3:
            printf("How do You Want to Delete Medication. Is it Based On?\n1. Medication_id\n2. Batch Number\n");
            int reply;
            scanf("%d", &reply);

            if (reply == 1)
            {
                // Deleting by Medication ID
                unsigned long id;
                printf("Enter Medication ID to delete: ");
                scanf("%lu", &id); 

                // Call the Delete_Medication function with Medication ID
                Delete_Medication(medications, total, id, NULL); // Pass NULL for batch 
                current_level--;
            }
            else if (reply == 2)
            {
                // Deleting by Batch Number
                char batch[BATCH_SIZE];
                printf("Enter Batch Number to delete: ");
                scanf("%s", batch); 

                // Call the Delete_Medication function with the batch number
                Delete_Medication(medications, total, -1, batch); 
                current_level--;
            }
            else
            {
                printf("Invalid Option!\n");
            } 
            char choice3;
            printf("Do you want to change this record into 'output.txt'? (y/n): ");
            scanf(" %c", &choice3);

            if (choice3 == 'y' || choice3=='Y') {
                WriteAllDataToFile("output.txt");  
            }
            printf("----------------------------------------------------------------------------------------------------------------------------\n");
            break;

        
        case 4:
    {
        unsigned long id_forSearch = 0;
        char medicineName_forSearch[NAME_SIZE] = "";
        unsigned long supplierID_forSearch = 0;

        printf("Enter Supplier ID (or type 0 if not applicable): ");
        scanf("%lu", &supplierID_forSearch);

        printf("Enter Medicine Name (or type 'none' if not applicable): ");
        scanf("%s", medicineName_forSearch);
        if (strcmp(medicineName_forSearch, "none") == 0) {
            strcpy(medicineName_forSearch, "");
        }

        printf("Enter Medication ID (or type 0 if not applicable): ");
        scanf("%lu", &id_forSearch);

        // Call Search_Medication with proper inputs
        Search_Medication(medications, total,id_forSearch != 0 ? id_forSearch : 0,
                        strcmp(medicineName_forSearch, "") != 0 ? medicineName_forSearch : NULL,
                        supplierID_forSearch != 0 ? supplierID_forSearch : 0);
        }
        printf("----------------------------------------------------------------------------------------------------------------------------\n");
        break;


        case 5:
            
            Generate_Alerts(medications, total);
            printf("----------------------------------------------------------------------------------------------------------------------------\n");
            break;

        case 6:
            printf("Enter the Day, Month and Year to check Expiration Dates: \n");
            int day, month, year;
            int tempday, tempmonth, tempyear;

            printf("Enter day, month, and year:\n");
            scanf("%d", &tempday);
            scanf("%d", &tempmonth);
            scanf("%d", &tempyear);

            year = tempyear;
            while (year < 1) {
                printf("Enter a valid Year: ");
                scanf("%d", &tempyear);
                year = tempyear; // Update year
            }

            month = tempmonth;
            while (month < 1 || month > 12) {
                printf("Enter a valid Month: ");
                scanf("%d", &tempmonth);
                month = tempmonth; // Update month
            }

            if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
                days_in_month[1] = 29; // February has 29 days in a leap year
            }

            // Ensure the day is valid for the given month and year
            day = tempday;
            while (day < 1 || day > days_in_month[month - 1]) { 
                printf("Enter a valid Day: ");
                scanf("%d", &tempday);
                day = tempday; // Update day
            }

            Check_Expiration_Dates(medications, total, day, month, year);
            printf("----------------------------------------------------------------------------------------------------------------------------\n");
            break;

        case 7:
            Sorting_ExpDate();
            char choice4;
            printf("Do you want to change this record into 'output.txt'? (y/n): ");
            scanf(" %c", &choice4);

            if (choice4 == 'y' || choice4=='Y') {
                WriteAllDataToFile("output.txt");  
            }
            printf("----------------------------------------------------------------------------------------------------------------------------\n");
            break;

        case 8:
            printf("Enter the Batch Name to track Sales: ");
            char BatchName_toTrack[BATCH_SIZE];
            scanf("%s", BatchName_toTrack);
            salesTracking(medications, total,BatchName_toTrack);
            printf("----------------------------------------------------------------------------------------------------------------------------\n");
            break;

        case 9:
            SupplierManagement(medications, total);
            char choice9;
            printf("Do you want to change this record into 'output.txt'? (y/n): ");
            scanf(" %c", &choice9);

            if (choice9 == 'y' || choice9=='Y') {
                WriteAllDataToFile("output.txt");  
            }
            printf("----------------------------------------------------------------------------------------------------------------------------\n");
            break;

        case 10:
            find_allrounder_suppliers(medications, total);
            printf("----------------------------------------------------------------------------------------------------------------------------\n");
            break;

        case 11:
            find_largest_turnover_suppliers(medications, total);
            printf("----------------------------------------------------------------------------------------------------------------------------\n");
            break;
        case 12:
             Printing();
             printf("----------------------------------------------------------------------------------------------------------------------------\n");
             break;
        default:
            printf("Invalid option. Try another option!\n");
            printf("----------------------------------------------------------------------------------------------------------------------------\n");
            break;
        }
    }
}