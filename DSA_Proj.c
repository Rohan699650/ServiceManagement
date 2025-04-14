#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<limits.h>

#define MAX_NAME_LENGTH 50
#define MAX_VEHICLE_NO_LENGTH 20
#define MAX_APPOINTMENT_DATE_LENGTH 20
#define MAX_SERVICE_CENTRES 10
#define MAX_LOCALITY_LENGTH 50
#define MAX_CUSTOMERS 100

struct ServiceLocation {
    char name[MAX_NAME_LENGTH];
    char locality[MAX_LOCALITY_LENGTH];
    float price;
    int rating;
};

struct Customer {
    int number;
    char vehicle_no[MAX_VEHICLE_NO_LENGTH];
    char appointment_date[MAX_APPOINTMENT_DATE_LENGTH];
    float price;
    char service_centre_name[MAX_NAME_LENGTH];
};

struct HashTable {
    int size;
    int *array;
};

struct HashTable hashTable;
struct ServiceLocation serviceCentres[MAX_SERVICE_CENTRES];
struct Customer customers[MAX_CUSTOMERS];


int adj[MAX_LOCALITY_LENGTH][MAX_LOCALITY_LENGTH]= {
        {99999, 1400, 240, 99999, 99999, 99999, 99999, 99999, 99999},
        {1400, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999},
        {240, 99999, 99999, 99999, 99999, 99999, 99999, 99999, 99999},
        {99999, 99999, 99999, 99999, 3800, 790, 99999, 99999, 99999},
        {99999, 99999, 99999, 3800, 99999, 99999, 99999, 99999, 99999},
        {99999, 99999, 99999, 790, 99999, 99999, 99999, 99999, 99999},
        {99999, 99999, 99999, 99999, 99999, 99999, 99999, 4100, 3800},
        {99999, 99999, 99999, 99999, 99999, 99999, 4100, 99999, 99999},
        {99999, 99999, 99999, 99999, 99999, 99999, 3800, 99999, 99999}};

char vertex_names[MAX_SERVICE_CENTRES][MAX_NAME_LENGTH];

void assign_name(void){
    strcpy(vertex_names[0], "Udyambagh");
    strcpy(vertex_names[1], "Manik_Enterprises");
    strcpy(vertex_names[2], "Car_care_centre next");
    strcpy(vertex_names[3], "Autonagar");
    strcpy(vertex_names[4], "Indian_AutoGarage");
    strcpy(vertex_names[5], "Dynamic_Autology");
    strcpy(vertex_names[6], "Bauxite_road");
    strcpy(vertex_names[7], "Infinity_Motor");
    strcpy(vertex_names[8], "Mahindra_Sotaria");
}

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Functions
int login();
void readServiceCentresFromFile();
void writeServiceCentresToFile();
void addServiceCentre();
void sortServiceCentresByPrice();
void sortServiceCentresByRating();
void sortServiceCentresByLocality();
void readCustomersFromFile();
void writeCustomersToFile();
void sortCustomersByVehicleNo();
void sortCustomersByPrices();
void sortCustomersByAppointmentDate();
void searchServiceCentresByLocality(const char *searchLocality);
void searchServiceCentreByName(const char *searchName);
int binarySearchCustomerByVehicleNo(const char *searchVehicleNo, int low, int high);
void readAdjacencyMatrixFromFile();
void swap(struct ServiceLocation *a, struct ServiceLocation *b);
int partition(struct ServiceLocation [], int , int);
void mergeSort(struct ServiceLocation [], int, int);
void merge(struct ServiceLocation [], int, int, int);
void heapify(struct Customer arr[], int n, int i);
void heapSort(struct Customer arr[], int n);
void dijkstra(int adj[][MAX_LOCALITY_LENGTH],int num_vertices, int source);


// Function to compare two appointment dates
int compareAppointmentDates(const char *date1, const char *date2) {
    return strcmp(date1, date2);
}

int main() {
    int choice;
    char choice1;
    do {
        int userType = login();
        if (userType == 1) { // Admin
            do {
                printf("\n<<**************** Admin Menu  ****************>>\n");
                printf("1. View all service centres\n");
                printf("2. Add a new service centre\n");
                printf("3. Add a new customer\n");
                printf("4. Display All Customers\n");
                printf("5. Sort service centres\n");
                printf("6. Sort customers\n");
                printf("7. Search for a customer\n");
                printf("8. Exit\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);


                switch (choice) {
                    case 1:
                        readServiceCentresFromFile();
                        break;
                    case 2:
                        addServiceCentre();
                        break;

                    case 3:
                        writeCustomersToFile();
                        break;

                    case 4:
                        displayCustomers();
                        break;
                    case 5:
                        printf("\n<<****************Sort service centres****************>>\n");
                        printf("\nSort service centres by:\n");
                        printf("a. Price\n");
                        printf("b. Rating\n");
                        printf("c. Locality\n");
                        printf("d. Return to Menu\n");
                        printf("Enter your choice: ");
                        scanf(" %c", &choice1);
                        do{switch (choice1) {
                            case 'a':
                                sortServiceCentresByPrice();
                                break;
                            case 'b':
                                sortServiceCentresByRating();
                                break;
                            case 'c':
                                sortServiceCentresByLocality();
                                break;
                            case 'd':
                                printf("Exiting\n");
                                break;

                            default:
                                printf("\n\n!!!!!!!!!ERROR!!!!!!!!!\n\n");
                                printf("Invalid choice. Please enter a valid option.\n");
                            }
                        }while(choice1!='d');
                        break;
                    case 6:
                        printf("\nSort customers by:\n");
                        printf("a. Vehicle Number\n");
                        printf("b. Prices\n");
                        printf("c. Appointment Date\n");
                        printf("d. Return to Menu\n");
                        printf("Enter your choice: ");
                        scanf(" %c", &choice1);
                        switch (choice1) {
                            case 'a':
                                sortCustomersByVehicleNo();
                                break;
                            case 'b':
                                sortCustomersByPrices();
                                break;
                            case 'c':
                                sortCustomersByAppointmentDate();
                                break;
                            case 'd':
                                printf("Exiting\n");
                                break;
                            default:
                                printf("\n\n!!!!!!!!!ERROR!!!!!!!!!\n\n");
                                printf("Invalid choice. Please enter a valid option.\n");
                        }

                        break;
                    case 7: {
                        char searchVehicleNo[MAX_VEHICLE_NO_LENGTH];
                        printf("Enter vehicle number to search: ");
                        scanf("%s", searchVehicleNo);
                        int index = binarySearchCustomerByVehicleNo(searchVehicleNo, 0, MAX_CUSTOMERS - 1);
                        if (index != -1) {
                            printf("Customer found:\n");
                            printf("Name: %s\n", customers[index].number);
                            printf("Vehicle Number: %s\n", customers[index].vehicle_no);
                            printf("Appointment Date: %s\n", customers[index].appointment_date);
                            printf("Price: %.2f\n", customers[index].price);
                            printf("Service Centre Name: %s\n", customers[index].service_centre_name);
                        } else {
                            printf("No matching customer found for vehicle number '%s'.\n", searchVehicleNo);
                        }
                        break;
                    }
                    case 8:
                        printf("\n<<<<<<<<<<<<<<<<<<<<<<Logging Out>>>>>>>>>>>>>>>>>>>>>\n");
                        printf("\n\n<<**************** Terminated  ****************>>\n\n");
                        break;
                    default:
                        printf("\n\n!!!!!!!!!ERROR!!!!!!!!!\n\n");
                        printf("Invalid choice. Please enter a valid option.\n");
                }
            } while (choice != 8);
        } else if (userType == 2) { // Customer
            do {
                printf("\n<<**************** Customer Menu ****************>>\n");
                printf("1. View all service centres\n");
                printf("2. Sort service centres\n");
                printf("3. Search Service Centre\n");
                printf("4. Calculate shortest distance\n");
                printf("5. Exit\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);

                switch (choice) {
                    case 1:
                        readServiceCentresFromFile();
                        break;
                    case 2:
                        printf("\nSort service centres by:\n");
                        printf("a. Price\n");
                        printf("b. Rating\n");
                        printf("c. Locality\n");
                        printf("d. Return to Menu\n");
                        printf("Enter your choice: ");
                        scanf(" %c", &choice1);
                        clearBuffer();
                        switch (choice1) {
                            case 'a':
                                sortServiceCentresByPrice();
                                break;
                            case 'b':
                                sortServiceCentresByRating();
                                break;
                            case 'c':
                                sortServiceCentresByLocality();
                                break;
                            case 'd':
                                printf("Exiting\n");
                                break;
                            default:
                                printf("\n\n!!!!!!!!!ERROR!!!!!!!!!\n\n");
                                printf("Invalid choice. Please enter a valid option.\n");
                        }
                        break;

                    case 3:
                        printf("a. Search Using Locality\n");
                        printf("b. Search Using Name\n");
                        printf("c. Return to Menu\n");
                        scanf(" %c", &choice1);
                        clearBuffer();
                        switch(choice1){
                            case 'a':{
                            char searchLocality[50];
                            printf("Enter locality to search: ");
                            scanf("%s", searchLocality);
                            searchServiceCentresByLocality(searchLocality);
                            break;
                            }

                        case'b':{
                            char searchName[50];
                            printf("Enter Name to Search: ");
                            scanf("%s", searchName);
                            searchServiceCentreByName(searchName);
                            break;
                        }

                        case 'c':{
                            printf("Exiting\n");
                            break;
                        }
                        default:{
                            printf("\n\n!!!!!!!!!ERROR!!!!!!!!!\n\n");
                            printf("Invalid Choice. Please enter a valid option\n");
                            break;

                        }

                        }

                    break;

                    case 4: {
                        char source[MAX_NAME_LENGTH];
                        assign_name();
                        printf("Enter the source vertex name: ");
                        getchar();  // Consume the newline character
                        fgets(source, MAX_NAME_LENGTH, stdin);
                        source[strcspn(source, "\n")] = '\0';
                        int num_vertices = 9;
                        // Map source name to its index in the matrix
                        int source_index = -1;
                        for (int i = 0; i < num_vertices; i++) {
                            if (strcmp(source, vertex_names[i]) == 0) {
                                source_index = i;
                                break;
                            }
                        }

                        if (source_index == -1) {
                            printf("Invalid source name!\n");
                            return 1;
                        }

                        dijkstra(adj, num_vertices, source_index);
                        break;
                    }

                    case 5:
                        printf("\n<<<<<<<<<<<<<<<<<<<<<<Logging Out>>>>>>>>>>>>>>>>>>>>>\n");
                        printf("\n\n<<**************** Terminated  ****************>>\n\n");
                        break;

                }

        }while(choice!=5);

        }

                else {
            printf("Invalid login credentials. Please try again.\n");
            printf("\n*************************************************\n");
            }
    } while (1);

    return 0;
}

int login() {
    char username[10], password[10];
    int ch;

    printf("\n1. Login\n");
    printf("2. Exit\n");
    scanf("%d", &ch);

    do{switch(ch){
        case 1:{
            printf("Enter username: ");
            scanf(" %s", username);
            printf("Enter password: ");
            scanf(" %s", password);

            printf("\n*************************************************\n");

            if (strcmp(username, "ADMIN") == 0 && strcmp(password, "adminpass") == 0) {
                printf("Login successful as ADMIN.\n");
                return 1;
            } else if (strcmp(username, "CUST") == 0 && strcmp(password, "custpass") == 0) {
                printf("Login successful as CUST.\n");
                return 2;
            } else {
                return 0;
            }
            printf("\n<<<<<<<<<<<<<<<<<<<<<<-------------------------------------------->>>>>>>>>>>>>>>>>>>>>>\n");
            break;
        }
        case 2:
            printf("\nExiting Program! Bye Bye\n");
            printf("\n<<<<<<<<<<<<<<<<<<<<<<-------------------------------------------->>>>>>>>>>>>>>>>>>>>>>\n");
            exit(0);
            break;

        default:
            printf("Invalid choice! Try Again!\n");
        }

    }while(ch!=2);
}

void readServiceCentresFromFile() {
    FILE *file = fopen("servicelocations.txt", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    printf("%-20s\t\t%-20s\t%-10s\t\t%s\n", "Service Center", "Locality", "Price", "Rating");
    printf("--------------------------------------------------------------------------------\n");

    int index = 0;
    while (fscanf(file, "%49[^,],%49[^,],%f,%d\n",
                  serviceCentres[index].name,
                  serviceCentres[index].locality,
                  &serviceCentres[index].price,
                  &serviceCentres[index].rating) == 4) {
        printf("%-20s%-20s%-10.2f%d\n", serviceCentres[index].name, serviceCentres[index].locality,
               serviceCentres[index].price, serviceCentres[index].rating);
        index++;
    }

    fclose(file);
}

void writeServiceCentresToFile() {
    FILE *file = fopen("servicelocations.txt", "w");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    for (int i = 0; i < MAX_SERVICE_CENTRES; i++) {
        fprintf(file, "%s,%s,%.2f,%d\n", serviceCentres[i].name, serviceCentres[i].locality,
                serviceCentres[i].price, serviceCentres[i].rating);
    }

    fclose(file);
}

void addServiceCentre() {
    for (int i = 0; i < MAX_SERVICE_CENTRES; i++) {
        if (serviceCentres[i].name[0] == '\0') {
            printf("Enter service center number: ");
            scanf("%s", serviceCentres[i].name);
            printf("Enter locality: ");
            scanf("%s", serviceCentres[i].locality);
            printf("Enter price: ");
            scanf("%f", &serviceCentres[i].price);
            printf("Enter rating: ");
            scanf("%d", &serviceCentres[i].rating);
            printf("Service center added successfully.\n");
            writeServiceCentresToFile();
            return;
        }
    }
    printf("Cannot add more service centres. The limit is %d.\n", MAX_SERVICE_CENTRES);
}

// Function to swap two ServiceLocation structures
void swap(struct ServiceLocation *a, struct ServiceLocation *b) {
    struct ServiceLocation temp = *a;
    *a = *b;
    *b = temp;
}

// Function to partition the array for QuickSort
int partition(struct ServiceLocation arr[], int low, int high) {
    float pivot = arr[high].price;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j].price <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

// Function to implement QuickSort algorithm
void quickSort(struct ServiceLocation arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void sortServiceCentresByPrice() {
    // Read service centres from file into the array
    FILE *file = fopen("servicelocations.txt", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    int index = 0;
    while (fscanf(file, "%49[^,],%49[^,],%f,%d\n",
                  serviceCentres[index].name,
                  serviceCentres[index].locality,
                  &serviceCentres[index].price,
                  &serviceCentres[index].rating) == 4) {
        index++;
    }

    fclose(file);

    // Perform QuickSort on the array based on price
    quickSort(serviceCentres, 0, index - 1);

    // Display the sorted service centres
    printf("%-20s%-20s%-10s%s\n", "Service Center", "Locality", "Price", "Rating");
    printf("----------------------------------------------------------\n");

    for (int i = 0; i < index; i++) {
        printf("%-20s%-20s%-10.2f%d\n", serviceCentres[i].name, serviceCentres[i].locality,
               serviceCentres[i].price, serviceCentres[i].rating);
    }
}

void searchServiceCentreByName(const char *searchName) {
    FILE *file = fopen("servicelocations.txt", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    struct ServiceLocation location;
    int matchFound = 0;

    printf("*****************************\n");
    printf("Matching service centers:\n");

    while (fscanf(file, "%49[^,],%49[^,],%f,%f\n", location.name, location.locality, &location.price, &location.rating) == 4) {
        // Compare the names using linear search
        if (strstr(location.name, searchName) != NULL) {
            printf("\n\nName: %s\nLocality: %s\nPrice: %.2f\nRating: %.2f\n", location.name, location.locality, location.price, location.rating);
            matchFound = 1;
        }
    }
    printf("*****************************\n");

    fclose(file);

    if (!matchFound) {
        printf("No matching service locations found for locality '%s'.\n\n", searchName);
    }
    printf("\n****************************************************************\n");
    printf("List of Other Service centres: \n");
    readServiceCentresFromFile();
    printf("\n****************************************************************\n");
}


void merge(struct ServiceLocation arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    // Create temporary arrays
    struct ServiceLocation L[n1], R[n2];

    // Copy data to temporary arrays L[] and R[]
    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Merge the temporary arrays back into arr[l..r]
    int i = 0; // Initial index of first subarray
    int j = 0; // Initial index of second subarray
    int k = l; // Initial index of merged subarray

    while (i < n1 && j < n2) {
        if (L[i].rating >= R[j].rating) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// l is for left index and r is the right index of the sub-array of arr to be sorted
void mergeSort(struct ServiceLocation arr[], int l, int r) {
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for large l and r
        int m = l + (r - l) / 2;

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        // Merge the sorted halves
        merge(arr, l, m, r);
    }
}

void sortServiceCentresByRating() {
    // Read service centres from file into the array
    FILE *file = fopen("servicelocations.txt", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    int index = 0;
    while (fscanf(file, "%49[^,],%49[^,],%f,%d\n",
                  serviceCentres[index].name,
                  serviceCentres[index].locality,
                  &serviceCentres[index].price,
                  &serviceCentres[index].rating) == 4) {
        index++;
    }

    fclose(file);

    // Perform MergeSort on the array based on rating
    mergeSort(serviceCentres, 0, index - 1);

    // Display the sorted service centres
    printf("%-20s%-20s%-10s%s\n", "Service Center", "Locality", "Price", "Rating");
    printf("----------------------------------------------------------\n");

    for (int i = 0; i < index; i++) {
        printf("%-20s%-20s%-10.2f%d\n", serviceCentres[i].name, serviceCentres[i].locality,
               serviceCentres[i].price, serviceCentres[i].rating);
    }
}

// Function to sort service centres by locality
void sortServiceCentresByLocality() {
    // Read service centres from file into the array
    FILE *file = fopen("servicelocations.txt", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    int index = 0;
    while (fscanf(file, "%49[^,],%49[^,],%f,%d\n",
                  serviceCentres[index].name,
                  serviceCentres[index].locality,
                  &serviceCentres[index].price,
                  &serviceCentres[index].rating) == 4) {
        index++;
    }

    fclose(file);

    // Perform bubble sort on the array based on locality
    for (int i = 0; i < index - 1; i++) {
        for (int j = 0; j < index - i - 1; j++) {
            // Compare localities using strcmp()
            if (strcmp(serviceCentres[j].locality, serviceCentres[j + 1].locality) > 0) {
                // Swap if the current locality is greater than the next one
                swap(&serviceCentres[j], &serviceCentres[j + 1]);
            }
        }
    }

    // Display the sorted service centres
    printf("%-20s%-20s%-10s%s\n", "Service Center", "Locality", "Price", "Rating");
    printf("----------------------------------------------------------\n");

    for (int i = 0; i < index; i++) {
        printf("%-20s%-20s%-10.2f%d\n", serviceCentres[i].name, serviceCentres[i].locality,
               serviceCentres[i].price, serviceCentres[i].rating);
    }
}

void searchServiceCentresByLocality(const char *searchLocality) {
    FILE *file = fopen("servicelocations.txt", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    struct ServiceLocation location;

    printf("%-20s%-20s%-10s%s\n", "Service Center", "Locality", "Price", "Ratings");
    printf("----------------------------------------------------------------------\n");

    int matchFound = 0;

    while (fscanf(file, "%49[^,],%49[^,],%f,%f\n", location.name, location.locality, &location.price, &location.rating) == 4) {
        // Use brute force string matching
        int i, j;
        for (i = 0; i <= strlen(location.locality) - strlen(searchLocality); i++) {
            for (j = 0; j < strlen(searchLocality) && location.locality[i + j] == searchLocality[j]; j++);
            if (j == strlen(searchLocality)) {
                // Found a match
                printf("%-20s%-20s%-10.2f%.2f\n", location.name, location.locality, location.price, location.rating);
                matchFound = 1;
                break;
            }
        }
    }

    fclose(file);

    if (!matchFound) {
        printf("No matching service locations found for locality '%s'.\n\n", searchLocality);
    }
    printf("\n****************************************************************\n");
    printf("List of Other Service centres: \n");
    readServiceCentresFromFile();
    printf("\n****************************************************************\n");
}

void readCustomersFromFile() {
    FILE *file = fopen("customers.txt", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    printf("%-20d%-15s%-20s%-10s%s\n", "Customer Number", "Vehicle No", "Appointment Date", "Price", "Service Centre");
    printf("-----------------------------------------------------------------------------------------------------\n");

    int index = 0;
    while (fscanf(file, "%49[^,],%19[^,],%19[^,],%f,%49[^\n]",
                  customers[index].number,
                  customers[index].vehicle_no,
                  customers[index].appointment_date,
                  &customers[index].price,
                  customers[index].service_centre_name) == 5) {
        printf("%-20d%-15s%-20s%-10.2f%s\n",
               customers[index].number,
               customers[index].vehicle_no,
               customers[index].appointment_date,
               customers[index].price,
               customers[index].service_centre_name);
        index++;
    }

    fclose(file);
}

// Function to write customers to file
void writeCustomersToFile() {
    FILE *file = fopen("customer.txt", "a");  // Open file in append mode
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    // Get customer details from the user (you can modify this part as needed)
    printf("Enter customer number: ");
    scanf("%d", customers[0].number);

    printf("Enter vehicle number: ");
    scanf("%s", customers[0].vehicle_no);

    printf("Enter appointment date: ");
    scanf("%s", customers[0].appointment_date);

    printf("Enter price: ");
    scanf("%f", &customers[0].price);

    printf("Enter service centre name: ");
    scanf("%s", customers[0].service_centre_name);

    // Write customer details to the file
    fprintf(file, "%d,%s,%s,%.2f,%s\n",
            customers[0].number,
            customers[0].vehicle_no,
            customers[0].appointment_date,
            customers[0].price,
            customers[0].service_centre_name);

    fclose(file);

    printf("Customer details written to customers.txt successfully.\n");
}

// Function to sort customers by vehicle number in ascending order
void sortCustomersByVehicleNo() {
    // Read customers from file into the array
    FILE *file = fopen("customer.txt", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    int index = 0;
    while (fscanf(file, "%d,%19[^,],%19[^,],%f,%49[^\n]",
                  &customers[index].number,
                  customers[index].vehicle_no,
                  customers[index].appointment_date,
                  &customers[index].price,
                  customers[index].service_centre_name) == 5) {
        index++;
    }

    fclose(file);

    // Perform bubble sort on the array based on vehicle number
    for (int i = 0; i < index - 1; i++) {
        for (int j = 0; j < index - i - 1; j++) {
            // Compare vehicle numbers using strcmp()
            if (strcmp(customers[j].vehicle_no, customers[j + 1].vehicle_no) > 0) {
                // Swap if the current vehicle number is greater than the next one
                swap(&customers[j], &customers[j + 1]);
            }
        }
    }

    // Print sorted customers
    printf("%-10s%-20s%-20s%-10s%s\n", "Number", "Vehicle No", "Appointment Date", "Price", "Service Centre");
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < index; i++) {
        printf("%-10d%-20s%-20s%-10.2f%s\n", customers[i].number, customers[i].vehicle_no,
               customers[i].appointment_date, customers[i].price, customers[i].service_centre_name);
    }
}



void displayCustomers() {
    FILE *file = fopen("customer.txt", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    struct Customer customer;

    printf("%-10s%-20s%-20s%-10s%s\n", "Number", "Vehicle No", "Appointment Date", "Price", "Service Centre");
    printf("------------------------------------------------------------\n");

    while (fscanf(file, "%d,%19[^,],%19[^,],%f,%49[^\n]\n", &customer.number, customer.vehicle_no,
                  customer.appointment_date, &customer.price, customer.service_centre_name) == 5) {
        printf("%-10d%-20s%-20s%-10.2f%s\n", customer.number, customer.vehicle_no, customer.appointment_date,
               customer.price, customer.service_centre_name);
    }

    fclose(file);
}

// Function to sort customers by prices in ascending order
// Function to sort customers by prices using Bubble sort
void sortCustomersByPrices() {
    // Read customers from file into the array
    FILE *file = fopen("customer.txt", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    int index = 0;
    while (fscanf(file, "%d,%19[^,],%19[^,],%f,%49[^\n]",
                  &customers[index].number,
                  customers[index].vehicle_no,
                  customers[index].appointment_date,
                  &customers[index].price,
                  customers[index].service_centre_name) == 5) {
        index++;
    }

    fclose(file);

    // Perform bubble sort on the array based on prices
    for (int i = 0; i < index - 1; i++) {
        for (int j = 0; j < index - i - 1; j++) {
            // Compare prices
            if (customers[j].price > customers[j + 1].price) {
                // Swap if the current price is greater than the next one
                swap(&customers[j], &customers[j + 1]);
            }
        }
    }

    // Display the sorted customers by prices
    printf("%-20s%-15s%-20s%-10s%s\n", "Name", "Vehicle No", "Appointment Date", "Price", "Service Centre");
    printf("------------------------------------------------------------------------\n");

    for (int i = 0; i < index; i++) {
        printf("%-20d%-15s%-20s%-10.2f%s\n",
               customers[i].number,
               customers[i].vehicle_no,
               customers[i].appointment_date,
               customers[i].price,
               customers[i].service_centre_name);
    }
}

// Function to sort customers by appointment date using Heap sort and Hash table
void sortCustomersByAppointmentDate() {
    // Read customers from file into the array
    FILE *file = fopen("customer.txt", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    int index = 0;
    while (fscanf(file, "%d,%19[^,],%19[^,],%f,%49[^\n]",
                  &customers[index].number,
                  customers[index].vehicle_no,
                  customers[index].appointment_date,
                  &customers[index].price,
                  customers[index].service_centre_name) == 5) {
        index++;
    }

    fclose(file);

    // Build a hash table for indexing customers by appointment date
    struct {
        int size;
        int *array;
    } hashTable;

    hashTable.size = index;
    hashTable.array = (int *)malloc(index * sizeof(int));
    for (int i = 0; i < index; i++) {
        hashTable.array[i] = i;
    }

    // Sort customers by appointment date using Heap sort
    heapSort(customers, index);

    // Display the sorted customers by appointment date
    printf("%-20s%-15s%-20s%-10s%s\n", "Name", "Vehicle No", "Appointment Date", "Price", "Service Centre");
    printf("------------------------------------------------------------------------\n");

    for (int i = 0; i < index; i++) {
        int originalIndex = hashTable.array[i];
        printf("%-20d%-15s%-20s%-10.2f%s\n",
               customers[originalIndex].number,
               customers[originalIndex].vehicle_no,
               customers[originalIndex].appointment_date,
               customers[originalIndex].price,
               customers[originalIndex].service_centre_name);
    }

    // Free allocated memory for the hash table
    free(hashTable.array);
}

// Heapify for Heap sort
void heapify(struct Customer arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && compareAppointmentDates(arr[left].appointment_date, arr[largest].appointment_date) > 0) {
        largest = left;
    }

    if (right < n && compareAppointmentDates(arr[right].appointment_date, arr[largest].appointment_date) > 0) {
        largest = right;
    }

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

// Heap sort
void heapSort(struct Customer arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}

int binarySearchCustomerByVehicleNo(const char *searchVehicleNo, int low, int high) {
    while (low <= high) {
        int mid = low + (high - low) / 2;

        int compareResult = strcmp(customers[mid].vehicle_no, searchVehicleNo);

        if (compareResult == 0) {
            // Match found, return the index
            return mid;
        } else if (compareResult < 0) {
            // If the search key is greater, ignore the right half
            low = mid + 1;
        } else {
            // If the search key is smaller, ignore the left half
            high = mid - 1;
        }
    }

    // If the loop exits, the element is not present
    return -1;
}


void dijkstra(int adj[][MAX_LOCALITY_LENGTH],int num_vertices, int source) {
    int visited[num_vertices];
    int distance[num_vertices];
    int parent[num_vertices];

    for (int i = 0; i < num_vertices; i++) {
        visited[i] = 0;
        distance[i] = INT_MAX;
        parent[i] = -1;
    }

    distance[source] = 0;

    for (int count = 0; count < num_vertices - 1; count++) {
        int min = INT_MAX, min_index;

        for (int v = 0; v < num_vertices; v++) {
            if (!visited[v] && distance[v] <= min) {
                min = distance[v];
                min_index = v;
            }
        }

        visited[min_index] = 1;

        for (int v = 0; v < num_vertices; v++) {
            if (!visited[v] && adj[min_index][v] && distance[min_index] != INT_MAX &&
                distance[min_index] + adj[min_index][v] < distance[v]) {
                distance[v] = distance[min_index] + adj[min_index][v];
                parent[v] = min_index;
            }
        }
    }

    printf("Closest car service center from source:\n");
    int min_distance = INT_MAX, closest_index;
    for (int i = 0; i < num_vertices; i++) {
        if (i != source && distance[i] < min_distance) {
            min_distance = distance[i];
            closest_index = i;
        }
    }

    printf("Index: %d, Distance: %d\n", closest_index, min_distance);

    // Print details of the closest center based on its name
    printf("Name: %s\n", vertex_names[closest_index]);
}

