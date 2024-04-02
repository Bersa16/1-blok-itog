#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
using namespace std;
class User 
{
public:
    string login;
    string password;
    string name;
    User(string l, string p, string n) : login(l), password(p), name(n) {}
};
class Chat 
{
private:
    vector<User> users; 
    map<string, vector<string>> messages; 
    User* findUserByLogin(const string& login) 
    {
        for (auto& user : users) 
        {
            if (user.login == login) 
            {
                return &user; 
            }
        }
        return nullptr; 
    }
public:
    void registerUser(const string& login, const string& password, const string& name) 
    {
        if (findUserByLogin(login) != nullptr) 
        {
            throw runtime_error("User with this login already exists.");
        }
        users.emplace_back(login, password, name);
        cout << "User \"" << name << "\" registered successfully.\n\n";
    }
    bool loginUser(const string& login, const string& password) 
    {
        User* user = findUserByLogin(login);
        if (user == nullptr || user->password != password) 
        {
            throw runtime_error("Login failed: Incorrect login or password.");
        }
        cout << "Welcome back, " << user->name << "!\n\n";
        return true;
    }
    void sendMessage(const string& from, const string& to, const string& message) 
    {
        if (findUserByLogin(from) == nullptr) 
        {
            throw runtime_error("Invalid sender.");
        }
        if (to != "All" && findUserByLogin(to) == nullptr) 
        {
            throw runtime_error("Invalid recipient.");
        }
        if (to == "All") 
        {
            for (auto& user : users) 
            {
                messages[user.login].push_back(from + ": " + message);
            }
        }
        else 
        {
            messages[to].push_back(from + ": " + message);
        }
        cout << "Message sent!\n\n";
    }
    void viewMessages(const string& login) 
    {
        if (messages.find(login) == messages.end() || messages[login].empty()) 
        {
            cout << "No messages for you.\n\n";
            return;
        }
        cout << "Messages for " << login << ":\n";
        for (const auto& message : messages[login]) 
        {
            cout << message << endl;
        }
        cout << endl;
    }
};
int main() 
{
    setlocale(LC_ALL, "Russian");

    Chat chat;
    chat.registerUser("alice", "123", "Alice Wonderland");
    chat.registerUser("bob", "456", "Bob Builder");
    chat.sendMessage("alice", "All", "Hello everyone! It's Alice.");
    chat.sendMessage("alice", "bob", "Hi Bob! This is Alice.");
    chat.sendMessage("bob", "alice", "Hello Alice! Bob here.");
    while (true) 
    {
        cout << "Welcome to the Simple Chat!\n";
        cout << "1. Register new user\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Select option: ";
        int option;
        cin >> option;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        if (option == 1) 
        {
            string login, password, name;
            cout << "Enter new login: ";
            getline(cin, login);
            cout << "Enter new password: ";
            getline(cin, password);
            cout << "Enter your name: ";
            getline(cin, name);
            try 
            {
                chat.registerUser(login, password, name);
            }
            catch (const runtime_error& e) 
            {
                cout << e.what() << endl;
            }
        }
        else if (option == 2) 
        {
            string login, password;
            cout << "Login: ";
            getline(cin, login);
            cout << "Password: ";
            getline(cin, password);
            try 
            {
                if (chat.loginUser(login, password)) 
                {
                    cout << "Logged in as " << login << ".\n";
                    chat.viewMessages(login);
                    string recipient, message;
                    cout << "Enter recipient ('All' for everyone, or specific login): ";
                    getline(cin, recipient);
                    cout << "Enter your message: ";
                    getline(cin, message);
                    chat.sendMessage(login, recipient, message);
                    cout << "Here are the latest messages:\n";
                    chat.viewMessages(login);
                }
            }
            catch (const runtime_error& e) 
            {
                cout << e.what() << endl;
            }
        }
        else if (option == 3) 
        {
            cout << "Goodbye!\n";
            break; 
        }
        else 
        {
            cout << "Invalid option. Please try again.\n";
        }
    }
    return 0;
}