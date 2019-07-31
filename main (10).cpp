#ifndef _GRID_WORLD_H
#define _GRID_WORLD_H
#include <vector>
#include <iostream>
using std::vector;
//question: Do i have to use typedef with structs
//dynamic arrays, linked lists, and queues

class GridWorld {
private:
    
    //TO DO maked doubly linked
    struct node{ //struct for one node in a linked list
        int idnum; //id number
        node *next; //pointer to next oldest person
        node *previous; //for doubly linked list
        node() : idnum(10), next(NULL), previous(NULL){ } //~Constructor
    };
    
    struct list{ //struct for front and back of a linked list
        node *front;
        node *back;
       list() : front(NULL), back(NULL) { } //~Constructor
     };
    
    struct district{ //struct for districts in temp vector in Gridworld() function
        int row; //row of district on grid
        int col; //col of district on grid
        int districtpop;
        int idnum;
        //linked list data
        list *head; //pointer to list structure
        district() : row(0), col(0), districtpop(0), head(NULL) { } //~Constructor
        
       // void constructor(){
        //    head->front->next = head->back;
       // }
    
    };
    
    struct human{ //struct for person in people vector
        int ID;
        bool alive;
        int row;
        int col;
        human() : ID(0), alive(0), row(0), col(0) { }
    };

    int rows=3; //unsigned makes sure population cannot be negative
    int cols=4;
    int size; //size of rows and cols 3x4
    int pop=0; //for population of gridworld
    
    //int row=3; //we need these for the whereis function to work for some reason?
   // int col=4;

    district **grid; //dynamic array of district structs
    //int IDarray[10]; //an array of 10 IDs
    vector<human> people; //vector of 'human' structs created in birth function.

public:
    
    struct node* getNewNode(int id){
        struct node* newNode = new node;
        (*newNode).idnum = id;
        (*newNode).previous = NULL;
        (*newNode).next = NULL;
        return newNode;
    }

    void push_front(int id, list *head ){
        struct node* newNode = getNewNode(id);
        if (head->front == NULL){ //if there is only head
            head->front = newNode;
            head->back = newNode;
            return;
        }
        
        head->front->previous = newNode;
        head->front->next = NULL;
        newNode->next = head->front;
        head->front = newNode;
    }

    
    bool popValue (int id, list *head){ //traverse the list unti value equals id. when value equals id, delete it and change next/previous pointers.
        list *p = head; //save node that is being deleted
        if (head->front == NULL){ //if list is empty
            std::cout << "List is empty" << std::endl;
            return false;
        }
        if (head->front->idnum == id){ //if front node is being deleted
            head->front = head->front->next;
            free (head);
            return true;
           // head->front->previous = NULL;
            std::cout << "Deleting front node" << std::endl;
        }
        if (head->front->next != NULL){
            head->front->next->previous = head->front->previous;
            free (head);
            return true;
        }
        if (head->front->previous != NULL){
            head->front->previous->next = head->front->next;
            free (head);
            return true;
        }
      
        free(p);
        return false;
    }
    
    GridWorld(unsigned nrows, unsigned ncols)   { //instead of pushing back ints, push back district struct?
        //vector<human> people; //vector of 'human' structs created in birth function.
        grid = new district*[nrows];
        int i;
        
        for (i=0; i<nrows; i++){ //populate *rows with cols
            grid[i] = new district[ncols];
        }
        i = 0;
        int j;
        
        for (i = 0; i < nrows; i++) { //create 3 vectors
            district *temp = new district; //creating temporary struct to push values on
            temp->head = new list;
            for (j =0; j< ncols; j++){ //populate each vector with 4 distruct structs
                temp->row = i;
                temp->col = j;
                grid[i][j] = *temp;
            }
   
        }
       
        vector<human> people; //vector of 'human' structs created in birth function.
    }
    
    ~GridWorld(){ //destructor sets population to 0, kills everybody
        int i, j;
         i= 0; j = 0;
        pop = 0; //set population back to zero
        for (i = 0; i<rows; i++){ //deallocating grid
            delete[] grid[i];
        }
        delete[] grid;
        i = 0;
        for (i = 0; i<rows ; i++){ //deallocate linked lists
            for (j=0; i < cols; j++){
                while (grid[i][j].head->front != NULL)
                {
                    node* tmpNode = grid[i][j].head->front;
                    delete grid[i][j].head->front;
                    grid[i][j].head->front = tmpNode;
                }
            }
        }
        //TO DO: insert kill function
        
    }

    /* function: birth
     * description:  if row/col is valid, a new person is created with an ID according to rules in handout.
     New person is placed in district (row, col)
     * return:  indicates success/failure
     
     */
    
    bool birth(int row, int col, int &id){
        
        if (row <= rows && col<= cols){  //first case: if district is valid, row & col will be <= size of gridworld
            id = pop+1;
            human person;
            person.ID = id;
            person.alive = 1;
            grid[row][col].row = row;
            grid[row][col].col = col;
            person.row = grid[row][col].row;
            person.col = grid[row][col].col;
            people.push_back(person); //add new human struct to people vector

            push_front(id, grid[row][col].head);

            pop++; //increase population size
            grid[row][col].districtpop++;

           // for (int i =0; i < pop-1; i++){
                     std::cout << "linked list: " << grid[row][col].head->front->idnum << " ";
            
                /// std::cout << "linked list: " << grid[row][col].head->back->idnum << " ";
               // std::cout << "linked list: " << grid[row][col].head->front->previous->idnum << " ";
                   //  std::cout << "linked list: " << grid[row][col].head->front->next->idnum << " ";
                     //   std::cout << "linked list: " << grid[row][col].head->back->previous->idnum << " ";
          //  }
            
            return true;
        
        }
        else { //if district is not valid
            std::cout << "That is not a valid district!" << std::endl;
            return false;
        }
        
        
    return false;
        
    }
    

    /*
     * function: death
     * description:  if given person is alive, person is killed and
     *   data structures updated to reflect this change.
     *
     * return:  indicates success/failure
     */
    
    bool death(int personID){
        if (people[personID-1].alive == 1){ //if person is alive
            //remove from people vector and linked list
            //find linked list node
            int row = people[personID-1].row;
            int col = people[personID-1].col;
            while(grid[row][col].head->front->idnum != personID){ //scan list for person in linked list
                grid[row][col].head->front = grid[row][col].head->front->next;  //head = head->next
            }
            std::cout << "Found person id: " << grid[row][col].head->front->idnum << std::endl;
            popValue(personID, grid[row][col].head); //pop from linked list
            people.erase(people.begin()+ (personID-1)); //erase from people vector
            people[personID-1].alive = 0; //change alive value to dead
            pop--;
            grid[row][col].districtpop--;
            return true;
        }
        else{
            std::cout << "Person is already dead." << std::endl;
            return false;
        }
        return false;
    }

    /*
     * function: whereis
     * description:  if given person is alive, his/her current residence
     *   is reported via reference parameters row and col.
     *search for id in linked list
     * return:  indicates success/failure
     */
    
    bool whereis(int id, int &row, int &col)const{
        
        if (people[id-1].alive == 1){
           // std::cout << "Location: " << *people[id].row << " " << grid[row][col].row << std::endl;
            row = people[id-1].row;
            col = people[id-1].col;
            
            return true;
        }
        if (people[id-1].alive == 0){
            std::cout << "Person is dead." << std::endl;
            return false;
            
        }
        return false;
    }
    
    
    
    /*
     
     * function: move
     * description:  if given person is alive, and specified target-row
     *   and column are valid, person is moved to specified district and
     *   data structures updated accordingly
     * return:  indicates success/failure
     * comment/note:  the specified person becomes the 'newest' member
     *   of target district (least seniority) --  see requirements of members().
     */
    
    bool move(int id, int targetRow, int targetCol){
        if (people[id-1].alive == true){
            int row = people[id-1].row; //get current row and col of person
            int col = people[id-1].col;
            while(grid[row][col].head->front->idnum != id){ //scan list for person in linked list
                grid[row][col].head->front = grid[row][col].head->front->next;  //head = head->next
            }
            std::cout << "found id in linked list" ;
            std::cout << grid[row][col].head->front->idnum << std::endl;
            people[id-1].row = targetRow; //change human struct values
            people[id-1].col = targetCol;
            push_front(id, grid[targetRow][targetCol].head); //insert into new linked list
            popValue(id, grid[row][col].head); //send in id and pointer to node that has id
            
            //deallocate head pointer from old linked list and change values
            
            return true;
        }
        else{
            return false;
        }
        
    }
    //linked list is already in order of seniority so need to use push_back 
    std::vector<int> * members(int row, int col)const{
        std::vector<int> * membersvect = nullptr;
        
        while (grid[row][col].head->front->next != NULL){
            std::cout << grid[row][col].head->front->idnum << std::endl;
           // (*membersvect).push_back(grid[row][col].head->front->idnum);
            grid[row][col].head->front = grid[row][col].head->front->next;
        }
     
        (*membersvect).push_back(grid[row][col].head->front->idnum);
        return membersvect;
    }
    
    /*
     
     * function: population
     
     * description:  returns the current (living) population of the world.
     
     */
    
    int population()const{
        
        return pop;
        
    }

    /*
     
     * function: population(int,int)
     
     * description:  returns the current (living) population of specified
     
     *   district.  If district does not exist, zero is returned
     
     */
    
    int population(int row, int col)const{
        
        return grid[row][col].districtpop;
        
    }

    /*
     
     * function: num_rows
     
     * description:  returns number of rows in world
     
     */
    
    int num_rows()const {
        
        return rows; //return variable for num rows
        
    }

    /*
     
     * function: num_cols
     
     * description:  returns number of columns in world
     
     */
    
    int num_cols()const {
        
        return cols;
        
    }
    
    
    
};



#endif