#include <iostream> //for input and output
#include <string> //for strings
#include <fstream> //for file input and output
using namespace std; //make my life easier
//used some help from stackoverflow and chatgpt to help with some of the adder/subtractor logic, and trimming whitespace, but everything else was written solely by me.
string trim(const string& str) { //deal with whitespace
    size_t first = str.find_first_not_of(" \t\n\r"); //remove beginning whitespace
    size_t last = str.find_last_not_of(" \t\n\r"); //remove ending whitespace
    if (first == string::npos || last == string::npos) // //if there is no whitespace, return empty string
        return ""; //return empty string
    return str.substr(first, last - first + 1); //return trimmed string
}
bool valid_double(string input) { //function to check if the string is a valid number
    int periods = 0; // Variable to count the number of periods
    int length = input.length(); //get the length of the string
    if (length == 0) return false; // Empty string is not a valid number
    int start = 0; //keep track if we need to start after the sign or not
    if (input[0] == '-' || input[0] == '+') { // Check for optional sign at the beginning
        start = 1; //change starting index number 
    }
    bool hasDigits = false; // Track if the string contains at least one digit
    for (int i = start; i < length; i++) { // Loop through each character in the string
        if (input[i] == '.') { // Check if the character is a period
            periods++;
            if (periods > 1) return false; // More than one period is invalid
            if (input[i]==length-1){
                return false; // Period at the end of the string is invalid
            }
        } 
        else if (isdigit(input[i])) { // Check if the character is a digit
            hasDigits = true; // Mark that we have seen a digit
        } 
        else { // Any other character is invalid
            return false;
        }
    }
    if (!hasDigits) { // if it's just a period, return false
        return false;
    }
    return true; // If all checks pass, it's a valid number
}
string isplitter(string input){ //function to isolate the whole number part of the double string
    string result; //string to hold the result
    int length=input.length(); //get the length of the string
    int i=0; //initalize i iterator
    while (i<length){ //iterate through the string
        if (input[i]=='.'){ //if we find a period
            break; //end while loop
        } 
        else { //otherwise
            result += input[i]; //add the character to the result string
            i++; //increment i to skip the character
        }
    }
    return result; //return the result string
}
string dsplitter(string input){ //remove the leading portion double string into just the decimal part
    string result; //create a string to hold the result
    int i=0; //initialize i iterator
    int length=input.length(); //get the length of the string
    while (i < length) { //while there are still characters in the string
        if (input[i] == '.') { //if we find a period
            i++; //increment i to skip the period
            while (i < length) { //while there are still characters in the string
                result += input[i]; //add the character to the result string
                i++; //increment i to skip the character
            }
            break; //break out of the loop
        } else {
            i++; //increment i to skip the character
        }
    }
    return result; //return the result string
}
string adder(string input1, string input2) { //function to add two strings together
    string int1 = isplitter(input1); //get the whole number portion of string 1
    string int2 = isplitter(input2); //get the whole number portion of string 2
    string frac1 = dsplitter(input1); //get the decimal portion of string 1
    string frac2 = dsplitter(input2); //get the decimal portion of string 2

    while (frac1.length() < frac2.length()) frac1 += '0'; //add zeroes to the end of the decimal portion of part 1 if needbe to make it easier to handle
    while (frac2.length() < frac1.length()) frac2 += '0'; //add zeroes to the end of the decimal portion of part 2 if needbe to make it easier to handle
    //decimal handling
    string fractional_result;
    int carry = 0; //value used for dealing with carry over
    for (int i = frac1.length() - 1; i >= 0; i--) { //loop through the decimal portion of string 1
        int digit1 = frac1[i] - '0'; //convert the character to an integer
        int digit2 = frac2[i] - '0'; //convert the character to an integer
        int sum = digit1 + digit2 + carry; //add the two digits together and add the carry over
        carry = sum / 10; //calculate the carry over
        fractional_result.insert(fractional_result.begin(), (sum % 10) + '0'); //insert the result into the string
    }
    //integer handling
    string integer_result; //create a result strinig
    int i = int1.length() - 1, j = int2.length() - 1; //loop through the integer portion of string 1
    while (i >= 0 || j >= 0 || carry) { //while there are still characters in the string
        int digit1=0; //initialize digit1 to 0
        int digit2=0; //initialize digit2 to 0
        if (i>=0) { //if there are still characters in string 1
            digit1 = int1[i] - '0'; //convert the character to an integer
            i--; //decrement i to move to the next character
        }
        if (j>=0) { //if there are still characters in string 2
            digit2 = int2[j] - '0'; //convert the character to an integer
            j--; //decrement j to move to the next character
        }
        int sum = digit1 + digit2 + carry; //add the two digits together and add the carry over
        carry = sum / 10; //calculate the carry over
        integer_result.insert(integer_result.begin(), (sum % 10) + '0'); //insert the result into the string
    }

    if (!fractional_result.empty()) { //if there are items in the decimal portion of the string
        return integer_result + "." + fractional_result; //concatenate them into one string, and return them
    } 
    else { //if nothing is in the decimal portion, just return the integer portion
        return integer_result; //return the integer portion of the string
    }
}

string subtracter(string string1, string string2) { //if string 1 is negative, run the subtractor
    bool negative = false; //initalize a boolean if we will need to add a negative to the end or not
    if (string1.length() < string2.length() || (string1.length() == string2.length() && string1 < string2)) { //if the length of string 1 is less than string 2, or they have an equal length, and string 1 is lexographically less than string 2
        swap(string1, string2); //swap the contents of the two strings to make subtraction easier
        negative = true; //set boolean to true so we add a negative to the final string.
    }
    string int1 = isplitter(string1); //get the whole number portion of string 1
    string int2 = isplitter(string2); //get the whole number portion of string 2
    string frac1 = dsplitter(string1); //get the decimal portion of string 1
    string frac2 = dsplitter(string2); //get the decimal portion of string 2
    while (frac1.length() < frac2.length()) frac1 += '0'; //add zeroes to the end of the decimal portion of part 1 if needbe to make it easier to handle
    while (frac2.length() < frac1.length()) frac2 += '0'; //add zeroes to the end of the decimal portion of part 2 if needbe to make it easier to handle

    //decimal handling
    string fractional_result; //string to hold the result of the decimal portion
    int borrow = 0; //borrow value used for dealing with carry over
    for (int i = frac1.length() - 1; i >= 0; i--) { //loop through the decimal portion of string 1
        int digit1 = frac1[i] - '0'; //create an integer from the character
        int digit2 = frac2[i] - '0'; //create a integer from the character
        int diff = digit1 - digit2 - borrow; //subtract the two digits and subtract the borrow value
        if (diff < 0) { //if the result is negative
            diff += 10; //add 10 to the result
            borrow = 1; //set the borrow value to 1
        } else { //if the result is positive
            borrow = 0; //set the borrow value to 0
        }
        fractional_result.insert(fractional_result.begin(), diff + '0'); //insert the result into the string
    }
    //integer handling
    string integer_result; //string to hold the integer result portion
    int i = int1.length() - 1; //initialize i to the end of string 1
    int j = int2.length() - 1; //initialize j to the end of string 2
    while (i >= 0 || j >= 0 || borrow) { //while there are still characters in the string and the borrow value is not 0
        int digit1=0; //initialize digit1 to 0
        int digit2=0; //initialize digit2 to 0
        if (i>=0) { //if i is greater than 0
            digit1 = int1[i] - '0'; //convert the character to an integer
            i--; //decrease i to move to the next character
        }
        if (j>=0) { //if j is greater than 0
            digit2 = int2[j] - '0'; //convert the character to an integer
            j--; //decrease j to move to the next character
        }
        int diff = digit1 - digit2 - borrow; //subtract the two digits and subtract the borrow value
        if (diff < 0) { //if the result is negative
            diff += 10; //add 10 to the result
            borrow = 1; //set the borrow value to 1
        } 
        else { //if it's positive
            borrow = 0; //no borrow value
        }
        integer_result.insert(integer_result.begin(), diff + '0'); //add it to the result string
    }
    while (integer_result.length() > 1 && integer_result[0] == '0') { //if the first character is a zero, and it is not the only character in the string
        integer_result.erase(integer_result.begin()); //remove it
    }
    if (!fractional_result.empty()) { //if there are items in the decimal portion of the string
        while (fractional_result.length() > 1 && fractional_result.back() == '0') { //if the last character is a zero, and it is not the only character in the string
            fractional_result.pop_back(); //remove it
        }
        if (!fractional_result.empty()) { //if there are still items in the decimal portion of the string
            integer_result += "." + fractional_result; //concatenate them into one string, and return them
        }
    }
    if (negative) { //if the negative boolean we set earlier is set to true
        integer_result.insert(integer_result.begin(), '-'); //add a negative symbol in front
    }
    return integer_result; //return the final string
}

string handler(string string1, string string2) { //function to handle the addition and subtraction of the two strings
    bool negative1=false; //initialize negative1 to false
    if (string1[0]=='-'){ //if the first string is negative
        negative1=true; //set negative1 to true
        string1=string1.substr(1); //remove the negative sign from the first string
    }
    if (string2[0]=='-'){ //if the second string is negative
        string2=string2.substr(1); //remove the negative sign from the second string
    }
    if (string1[0]=='+'){ //if the first string is positive
        string1=string1.substr(1); //remove the positive sign from the first string
    }
    if (string2[0]=='+'){ //if the second string is positive
        string2=string2.substr(1); //remove the positive sign from the second string
    }
    while (string1[0]=='0' && string1.length()>1){ //if the first string is a zero, and it is not the only character in the string
        string1.erase(string1.begin()); //remove it
    }
    if (negative1){ //if both strings are negative
        return "-"+adder(string1,string2); //return the sum of the two strings with a negative sign
    }
    else{ //if the first string is positive and the second string is negative
        return subtracter(string1,string2); //return the sum of the two strings without a negative sign
    }
}
int main() { //main function
    string name; // Create a string to hold the name of the file
    cout << "What is the name of the file containing the numbers?" << endl; // Prompt user
    cin >> name; // Take in a file name
    ifstream file(name); // Open file
    string total;
    if (!file.is_open()) { //if we can't open the file
        cout << "Error opening file, file may not exist" << endl; //error handling
        return 0; //end program
    }
    while (file.is_open()) { // Check if file is open
        string num; // Create a string to hold each line
        if (getline(file, num)) {//take in the first line to start the running total
            num = trim(num); // Trim the string
            if (valid_double(num)) { // If the string is a valid double
                total=handler(num,"-123.456"); //add -123.456 from the first number
                cout << "Total sum of valid numbers: " << total << endl; //print total
            } 
            else { //if it fails the double check
                cout << "Invalid number: " << num << endl; // Print the invalid number
            }
        }
        else{
            return 0;
        }
    }
    return 0;
}