#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <ctime>
using namespace std;

const int MAX_MOVIES = 100;

string currentUser = "";

bool loginUser() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    ifstream file("users.txt");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string storedUser, storedPass;
        getline(ss, storedUser, '|');
        getline(ss, storedPass);
        if (storedUser == username && storedPass == password) {
            currentUser = username;
            cout << "Login successful. Welcome, " << username << "!\n";
            return true;
        }
    }
    cout << "Invalid credentials.\n";
    return false;
}

bool signupUser() {
    string username, password;
    cout << "Choose a username: ";
    cin >> username;

    ifstream infile("users.txt");
    string line;
    while (getline(infile, line)) {
        stringstream ss(line);
        string existingUser;
        getline(ss, existingUser, '|');
        if (existingUser == username) {
            cout << "Username already taken. Please choose another.\n";
            return false;
        }

    }

    cout << "Choose a password: ";
    cin >> password;

    ofstream outfile("users.txt", ios::app);
    outfile << username << "|" << password << "\n";

    ofstream userFile(username + "_transactions.txt");
    userFile.close();

    currentUser = username;
    cout << "Signup successful!\n";
    return true;
}

void logTransaction(const string& username, const string& action, const string& title, double amount) {
    ofstream file(username + "_transactions.txt", ios::app);
    time_t now = time(0);
    string date = ctime(&now);
    date.pop_back(); // Remove newline
    file << action << "|" << title << "|" << amount << "|" << date << "\n";
}


class Movie {
private:
    int id;
    string title;
    string genre;
    int duration;
    double rating;
    bool isRented;
    bool isPurchased;
    double rentCost;
    double purchaseCost;
    string rentDate;
    string returnDate;

public:
    Movie() : id(0), title(""), genre(""), duration(0), rating(0.0), isRented(false), isPurchased(false), rentCost(0.0), purchaseCost(0.0), rentDate(""), returnDate("") {}

    Movie(int id, string& title, string& genre, bool isRented, int duration, double rating, double rentCost, double purchaseCost)
    : id(id), title(title), genre(genre), isRented(isRented), duration(duration), rating(rating),
      rentCost(rentCost), purchaseCost(purchaseCost) {}


    int getId() const { return id; }
    string getTitle() const { return title; }
    string getGenre() const { return genre; }
    bool rented() const { return isRented; }
    int getDuration() const { return duration;}
    double getRating() const {return rating ;}
    bool getIsPurchased() const { return isPurchased; }
    void setIsPurchased(bool purchased) { isPurchased = purchased; }

    double getRentCost() const { return rentCost; }
    void setRentCost(double cost) { rentCost = cost; }

    double getPurchaseCost() const { return purchaseCost; }
    void setPurchaseCost(double cost) { purchaseCost = cost; }

    string getRentDate() const { return rentDate; }
    void setRentDate(const string& date) { rentDate = date; }

    string getReturnDate() const { return returnDate; }
    void setReturnDate(const string& date) { returnDate = date; }

    void rent() { isRented = true; }
    void returnMovie() { isRented = false; }
    bool purchased() const { return isPurchased; }


    string serialize() const {
    return to_string(id) + "|" + title + "|" + genre + "|" +
           (isRented ? "1" : "0") + "|" +
           to_string(duration) + "|" +
           to_string(rating) + "|" +
           to_string(rentCost) + "|" +
           to_string(purchaseCost);
}

    void display() const {
        cout << id << ". " << title << " (" << genre << ") [" << (isRented ? "RENTED" : "AVAILABLE") << "] "
             << "[" << (isPurchased ? "PURCHASED" : "NOT PURCHASED") << "] "
             << "(" << duration << " min) [" << rating << "/5.0] "
             << "Rent Cost: $" << rentCost << ", Purchase Cost: $" << purchaseCost << "\n";}

    void set(int newId, string& newTitle, string& newGenre, bool status, int new_duration, double new_rating, double new_rentCost, double new_purchaseCost) {
    id = newId;
    title = newTitle;
    genre = newGenre;
    isRented = status;
    duration = new_duration;
    rating = new_rating;
    rentCost = new_rentCost;
    purchaseCost = new_purchaseCost;
}
};

class MovieManager {
private:
    Movie movies[MAX_MOVIES];
    int movieCount;
    const string fileName = "movies.txt";

public:
    MovieManager() {
        movieCount = 0;
        loadMovies();
    }

    void loadMovies() {
    movieCount = 0;
    ifstream file(fileName);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line) && movieCount < MAX_MOVIES) {
        stringstream ss(line);
        string idStr, title, genre, statusStr, durStr, ratStr, rentCostStr, purchaseCostStr;

        if (getline(ss, idStr, '|') &&
            getline(ss, title, '|') &&
            getline(ss, genre, '|') &&
            getline(ss, statusStr, '|') &&
            getline(ss, durStr, '|') &&
            getline(ss, ratStr, '|') &&
            getline(ss, rentCostStr, '|') &&
            getline(ss, purchaseCostStr)) {

            int id = stoi(idStr);
            bool status = (statusStr == "1");
            int dur = stoi(durStr);
            double rat = stod(ratStr);
            double rentCost = stod(rentCostStr);
            double purchaseCost = stod(purchaseCostStr);

            movies[movieCount++].set(id, title, genre, status, dur, rat, rentCost, purchaseCost);
        }
    }

    file.close();
}
    void saveMovies() const {
    ofstream file(fileName);
    for (int i = 0; i < movieCount; ++i) {
        file << movies[i].serialize() << '\n';
    }
    file.close();

}

    

    void browseMovies() const {
        cout << "\n--- Movie List ---\n";
        for (int i = 0; i < movieCount; ++i) {
            movies[i].display();
        }
    }

    void rentMovie(int id) {
    for (int i = 0; i < movieCount; ++i) {
        if (movies[i].getId() == id) {
            if (!movies[i].rented()) {
                movies[i].rent();
                // Set rent date to current date
                time_t now = time(0);
                tm* ltm = localtime(&now);
                char rentDate[11];
                strftime(rentDate, sizeof(rentDate), "%Y-%m-%d", ltm);
                movies[i].setRentDate(rentDate);

                // Set return date to 10 days from now
                now += 10 * 24 * 60 * 60; // Add 10 days in seconds
                ltm = localtime(&now);
                char returnDate[11];
                strftime(returnDate, sizeof(returnDate), "%Y-%m-%d", ltm);
                movies[i].setReturnDate(returnDate);

                saveMovies();
                cout << "You rented \"" << movies[i].getTitle() << "\". Return by " << movies[i].getReturnDate() << ".\n";
                logTransaction(currentUser, "Rent", movies[i].getTitle(), movies[i].getRentCost());
            } else {
                cout << "That movie is already rented.\n";
            }
            return;
        }
    }
    cout << "Movie not found.\n";
}
    void purchaseMovie(int id) {
    for (int i = 0; i < movieCount; ++i) {
        if (movies[i].getId() == id) {
            if (!movies[i].getIsPurchased()) {
                movies[i].setIsPurchased(true);
                saveMovies();
                cout << "You purchased \"" << movies[i].getTitle() << "\" for $" << movies[i].getPurchaseCost() << ".\n";
                logTransaction(currentUser, "Purchase", movies[i].getTitle(), movies[i].getPurchaseCost());
            } else {
                cout << "You have already purchased this movie.\n";
            }
            return;
        }
    }
    cout << "Movie not found.\n";
}

void viewRentedMovies() const {
    cout << "\n--- Currently Rented Movies ---\n";
    bool found = false;
    for (int i = 0; i < movieCount; ++i) {
        if (movies[i].rented()) {
            movies[i].display();
            cout << "Rented on: " << movies[i].getRentDate() << ", Return by: " << movies[i].getReturnDate() << "\n";
            found = true;
        }
    }
    if (!found) {
        cout << "No movies are currently rented.\n";
    }
}

void viewPurchasedMovies() const {
    cout << "\n--- Purchased Movies ---\n";
    bool found = false;
    for (int i = 0; i < movieCount; ++i) {
        if (movies[i].purchased()) {
            movies[i].display();
            found = true;
        }
    }
    if (!found) {
        cout << "No movies have been purchased.\n";
    }
}

void viewTotalCharges() const {
    double totalRent = 0.0, totalPurchase = 0.0;

    for (int i = 0; i < movieCount; ++i) {
        if (movies[i].rented()) {
            totalRent += movies[i].getRentCost();
        }
        if (movies[i].purchased()) {
            totalPurchase += movies[i].getPurchaseCost();
        }
    }

    cout << "\n--- Charges Summary ---\n";
    cout << "Total Rent Charges: $" << totalRent << "\n";
    cout << "Total Purchase Charges: $" << totalPurchase << "\n";
    cout << "-------------------------\n";
    cout << "Total Amount Due: $" << (totalRent + totalPurchase) << "\n";
}

    void returnMovie(int id) {
        for (int i = 0; i < movieCount; ++i) {
            if (movies[i].getId() == id) {
                if (movies[i].rented()) {
                    movies[i].returnMovie();
                    saveMovies();
                    cout << "You returned \"" << movies[i].getTitle() << "\".\n";
                } else {
                    cout << "The movie wasn't rented.\n";
                }
                return;
            }
        }
        cout << "Movie not found.\n";
    }

    void addMovie(int id, string& title, string& genre, int dur, double rat, double rentCost, double purchaseCost) {
    if (movieCount >= MAX_MOVIES) {
        cout << "Movie list is full.\n";
        return;
    }
    for (int i = 0; i < movieCount; ++i) {
        if (movies[i].getId() == id) {
            cout << "Movie ID already exists.\n";
            return;
        }
    }
    movies[movieCount++].set(id, title, genre, false, dur, rat, rentCost, purchaseCost);
    saveMovies();
    cout << "Movie added.\n";
}
    void deleteMovie(int id) {
        bool found = false;
        for (int i = 0; i < movieCount; ++i) {
            if (movies[i].getId() == id) {
                found = true;
                for (int j = i; j < movieCount - 1; ++j) {
                    movies[j] = movies[j + 1];
                }
                movieCount--;
                saveMovies();
                cout << "Movie deleted.\n";
                break;
            }
        }
        if (!found) cout << "Movie not found.\n";
    }

    void adminViewUserCharges() {
    string username;
    cout << "Enter username to view charges: ";
    cin >> username;

    ifstream file(username + "_transactions.txt");
    if (!file.is_open()) {
        cout << "No transaction file for user.\n";
        return;
    }

    string line;
    double total = 0;
    cout << "\n--- Transactions for " << username << " ---\n";
    while (getline(file, line)) {
        stringstream ss(line);
        string action, title, amountStr, date;
        getline(ss, action, '|');
        getline(ss, title, '|');
        getline(ss, amountStr, '|');
        getline(ss, date);

        double amount = stod(amountStr);
        cout << action << ": " << title << " - $" << amount << " on " << date << "\n";
        total += amount;
    }
    cout << "Total Charges: $" << total << "\n";
}

    void searchByTitle(const string& title) const {
        cout << "\n--- Search Results (Exact Title: \"" << title << "\") ---\n";
        for (int i = 0; i < movieCount; ++i) {
            if (movies[i].getTitle() == title) {
                movies[i].display();
                return;
            }
        }
        cout << "No matching movies found.\n";
    }

    void searchByGenre(const string& genre) const {
    cout << "\n--- Movies in Genre: " << genre << " ---\n";
    bool found = false;
    for (int i = 0; i < movieCount; ++i) {
        if (movies[i].getGenre() == genre) {
            movies[i].display();
            found = true;
        }
    }
    if (!found)
        cout << "No movies found in this genre.\n";
}

};

// ---------- Interface Code ----------
void showUserMenu() {
    cout << "\n--- User Menu ---\n";
    cout << "1. Browse Movies\n";
    cout << "2. Rent Movie\n";
    cout << "3. Return Movie\n";
    cout << "4. Purchase Movie\n";
    cout << "5. View Total Charges\n";
    cout << "6. View Rented Movies\n";
    cout << "7. View purchased movies\n";
    cout << "8. Search by Title\n";
    cout << "9. Search by Genre\n";
    cout << "10. Logout\n";
    cout << "Enter your choice: ";
}

void adminMenu(MovieManager& manager) {
    string password;
    cout << "Enter admin password: ";
    cin >> password;

    if (password != "admin123") {
        cout << "Incorrect password.\n";
        return;
    }

    int choice;
    do {
        cout << "\n--- Admin Menu ---\n";
        cout << "1. Add Movie\n";
        cout << "2. Delete Movie\n";
        cout << "3. View All Movies\n";
        cout << "4. Check charges due for any user\n";
        cout << "5. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
case 1: 
    {int id, dur;
    string title, genre;
    double rat, rentCost, purchaseCost;

    cout << "Enter new movie ID: ";
    cin >> id;
    cin.ignore();

    cout << "Enter movie title: ";
    getline(cin, title);

    cout << "Enter genre: ";
    getline(cin, genre);

    cout << "Enter the duration in minutes: ";
    cin >> dur;

    cout << "Enter the rating: ";
    cin >> rat;

    cout << "Enter the rent cost: ";
    cin >> rentCost;

    cout << "Enter the purchase cost: ";
    cin >> purchaseCost;

    manager.addMovie(id, title, genre, dur, rat, rentCost, purchaseCost);
    break;
    }
            
            case 2: {
                int id;
                cout << "Enter movie ID to delete: ";
                cin >> id;
                manager.deleteMovie(id);
                break;
            }
            case 3:
                manager.browseMovies();
                break;
            case 4:
                manager.adminViewUserCharges();
                break;
            case 5:
                cout << "Logging out from admin.\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 5);
}

int main() {
    MovieManager manager;

    int mode;
    do {
        cout << "\n1. User SignUp\n2. User Login\n3. Admin\n4. Exit \n----------------\nChoose: ";
        cin >> mode;

        if (mode == 1) {
          if(signupUser()){
            int choice;
            do {

                showUserMenu();
                cin >> choice;
                cin.ignore();

                switch (choice) {
                    case 1:
                        manager.browseMovies();
                        break;
                    case 2: {
                        int id;
                        cout << "Enter movie ID to rent: ";
                        cin >> id;
                        manager.rentMovie(id);
                        break;
                    }
                    case 3: {
                        int id;
                        cout << "Enter movie ID to return: ";
                        cin >> id;
                        manager.returnMovie(id);
                        break;
                    }
                    case 4: {
                        int id;
                        cout << "Enter movie id to purchase: ";
                        cin>>id;
                        manager.purchaseMovie(id);
                        break;
                    }
                    case 5: {
                        manager.viewTotalCharges();
                        break;
                    }
                    case 6: {
                        manager.viewRentedMovies();
                        break;
                    }
                    case 7:
                    {
                        manager.viewPurchasedMovies();
                        break;
                    }
                    case 8: {
                        string title;
                        cout << "Enter movie title: ";
                        getline(cin, title);
                        manager.searchByTitle(title);
                        break;
                    }        

                    case 9: {
                        string genre;
                        cout << "Enter genre: ";
                        getline(cin, genre);
                        manager.searchByGenre(genre);
                        break;
                    }
                    case 10:
                        cout << "Logging out.\n";
                        break;
                    default:
                        cout << "Invalid option.\n";
                }
            } while (choice != 10);
        }
        }
        else if(mode==2)
        {
            if(loginUser())
            {
                
            int choice;
            do {

                showUserMenu();
                cin >> choice;
                cin.ignore();

                switch (choice) {
                    case 1:
                        manager.browseMovies();
                        break;
                    case 2: {
                        int id;
                        cout << "Enter movie ID to rent: ";
                        cin >> id;
                        manager.rentMovie(id);
                        break;
                    }
                    case 3: {
                        int id;
                        cout << "Enter movie ID to return: ";
                        cin >> id;
                        manager.returnMovie(id);
                        break;
                    }
                    case 4: {
                        int id;
                        cout << "Enter movie id to purchase: ";
                        cin>>id;
                        manager.purchaseMovie(id);
                        break;
                    }
                    case 5: {
                        manager.viewTotalCharges();
                        break;
                    }
                    case 6: {
                        manager.viewRentedMovies();
                        break;
                    }
                    case 7:
                    {
                        manager.viewPurchasedMovies();
                        break;
                    }
                    case 8: {
                        string title;
                        cout << "Enter movie title: ";
                        getline(cin, title);
                        manager.searchByTitle(title);
                        break;
                    }        

                    case 9: {
                        string genre;
                        cout << "Enter genre: ";
                        getline(cin, genre);
                        manager.searchByGenre(genre);
                        break;
                    }
                    case 10:
                        cout << "Logging out.\n";
                        break;
                    default:
                        cout << "Invalid option.\n";
                }
            } while (choice != 10);
        
            }
        }
        else if (mode == 3) {
            adminMenu(manager);
        }
        else if (mode == 4) {
            cout << "Exiting...\n";
        }
        else {
            cout << "Invalid selection.\n";
        }
    } while (mode != 4);

    return 0;
}
