# WiTablesIO
This C-library is made to display interactive tables in the terminal, 
and get the selected row back.

As special feature it also can display an extra box right of/under the table.
The alignment happens based on the terminal width at the start of the program.
It will see if there is enough space on the right, else it goes on under.


## Usage
Clone this repo, `cd` into it, and run `make`.
This will compile the code into `witablesio.a`, which you can find in `lib/`.

Now you can link the library. You can grab the header file in `include/` to
import it in your source code.

The details of the structure can be found in the header file, 
but you can create an empty table with
```c 
wiTable* table = wiCreateTable("Table title");
```

At the end, you need to free the table with `wiFreeTable(table)`.

To populate the table with `wiRow`s, 
