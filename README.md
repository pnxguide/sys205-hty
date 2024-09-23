# SYS-205 Assessment 3 - The hearty file format (`.hty`)
You will be writing several programs to process the `.hty` file format, an application-specific file format for data analytics.

## Acknowledgement
*This assessment is inspired from a part of [Project 1](https://15721.courses.cs.cmu.edu/spring2023/project1.html) of the CMU 15-721 Advanced Database System (Fall 23) course.*

## The `.hty` File Format
The `.hty` file format is a columnar file format (to be precise, it is PAX (or *partition attribute across*)), similarly to the famous [Apache Parquet](https://parquet.apache.org/), and [Apache ORC](https://orc.apache.org/). The file is specifically used to store columns of data. However, to accelerate data analytical tasks, the file groups multiple columns together into a *column group*. Usually, columns that are used together will be grouped in the same group.

The `.hty` file format is very simple that it only contains three components:

```
[Raw Data] [Metadata] [Metadata Size (4 bytes)]
```

More simply, all the data in this format will be 32-bit **signed** integers or floating points **except** the header, which will be a string.

To read this format, you need to read the `[Metadata size]` first. Then, you can read the `[Metadata]` to find the way to access the `[Raw Data]`. The `[Metadata]` is specified as follows:

```json
{
  "num_rows": the number of rows (32-bit integer),
  "num_groups": the number of groups (32-bit integer),
  "groups": [
    {
      "num_columns": the number of columns in the groups (32-bit integer),
      "offset": the offset in the file for the start of this column group (32-bit integer),
      "columns": [
        {
          "column_name": the column name (string),
          "column_type": the type of the column (string whether "int" | "float")
        },
        {
          ...
        }
      ]
    },
    {
      ...
    }
  ]
}
```

The data in the `[Raw Data]` component will be layed out as contiguous bytes. For example, given a column group specified below:

```
{
  "num_columns": 3,
  "offset": 0,
  "columns": [
    {
      "column_name": "id",
      "column_type": "int"
    },
    {
      "column_name": "type",
      "column_type": "int"
    },
    {
      "column_name": "salary",
      "column_type": "float"
    }
  ]
}
```

Given several rows of data in the column group stored in the `.csv` format:

```
1,1,25000.0
2,1,30000.0
3,1,27000.0
4,2,80000.0
```

The data stored in the raw data section must be packed as follows:

```
000000000000000000000000000000010000000000000000000000000000000101000110110000110101000000000000000000000000000000000000000000100000000000000000000000000000000101000110111010100110000000000000000000000000000000000000000000110000000000000000000000000000000101000110110100101111000000000000000000000000000000000000000001000000000000000000000000000000001001000111100111000100000000000000
```

Note that the bit sequence above can be interpreted as:

```
[1][1][25000.0][2][1][30000.0][3][1][27000.0][4][2][80000.0]
```

where each block of `[]` represents a group of 32 bits.

## Task #1 - Convert `.csv` to `.hty` (20 points)
You need to write a function to convert a specialized `.csv` file, whose data only are integers and decimals, into a `.hty` file. You need to explicitly write down the `.hty` file on your machine.

In the file `src/csv_to_hty.cpp`, you will need to implement the following function.

```cpp
void convert_from_csv_to_hty(std::string csv_file_path, std::string hty_file_path);
```

## Task #2 - Extract the metadata (10 points)
You need to write a function to extract the metadata of the file and store it into a memory. You may want to use a nice tool like [nlohmann/json](https://github.com/nlohmann/json) to help handle JSON.

In summary, you need to implement the following function in the `src/analyze.cpp`.

```cpp
nlohmann::json extract_metadata(std::string hty_file_path);
```

Please verify the implementation well since you will need to use this in the following tasks.

## Task #3 - Project and display a single column (15 points)
You need to write a function to read a column of interest from the `.hty` file. 

### Task #3.1 - Project a column (10 points)
Basically, you should implement the function with the following function signature.

```cpp
std::vector<int> project_single_column(nlohmann::json metadata, std::string hty_file_path, std::string projected_column);
```

For both integer and floating-point columns, you should use the same function to extract.

For one who understands SQL, this is similar to the following SQL statement:

```sql
SELECT projected_column
FROM hty_file_path;
```

### Task #3.2 - Display a column (5 points)
However, you may wonder since the return value of the `extract_single_column` is `std::vector<int>`. Displaying an integer column should be straightforward. For a floating-point column, you need to check whether the column is the floating-point or not. Then, you can cast a type of an integer into a floating-point. All of these should be done under the following function.

```cpp
void display_column(nlohmann::json metadata, std::string column_name, std::vector<int> data);
```

Given an example of `data` as `[5, 10, 15]`. The function should display in the following format:

```
column_name
5
10
15
```

## Task #4 - Filter on a single column (15 points)
You need to write a function to keep only a value of interest in a column from the `.hty` file. This is similar to the following SQL statements.

```sql
SELECT projected_column
FROM hty_file_path
WHERE projected_column [operation] [value];
```

You will need to support 6 `operation`s: `>`, `>=`, `<`, `<=`, `=`, and `!=`. The `value` can be anything with the same type as the column_name. However, you do not need to check the type of the `value`. You just need to cast it to be the same as the type of the `column_name`.

The function you will be working on is:

```cpp
std::vector<int> filter(nlohmann::json metadata, std::string hty_file_path, std::string projected_column, int operation, int filtered_value);
```

## Task #5 - Project and display multiple columns from the same group (15 points)

### Task 5.1 - Project multiple columns from the same group (10 points)
You need to write a function to read multiple columns of interest from the `.hty` file. This is similar to the following SQL statements.

```sql
SELECT column_name_1, column_name_2, ..., column_name_n
FROM hty_file_path;
```

However, in the actual SQL, you can also project the same column multiple times, in which you need to support this ability as well.

The function you will be working on is:

```cpp
std::vector<std::vector<int>> project(nlohmann::json metadata, std::string hty_file_path, std::vector<std::string> projected_columns);
```

You must ensure that all the columns in the `projected_columns` are in the same column group.

### Task 5.2 - Display multiple columns (5 points)
Similar to Task 3.2, but this time, you need to display multiple columns. We can call them a result set.

```cpp
void display_result_set(nlohmann::json metadata, std::vector<std::string> column_name, std::vector<std::vector<int>> result_set);
```

Given an example of `result_set` as `[[1, 2], [5.0, 6.7]]`. The function should display in the following format:

```
column_name_1, column_name_2
1,5.0
2,6.7
```

## Task #6 - Filter based on a column from the same group (10 points)
You need to write a function to read columns of interest and filter some of the rows by the filtering condition that may not related to the projected columns. This is similar to the following SQL statements.

```sql
SELECT column_name_1, column_name_2, ...
FROM hty_file_path
WHERE filtered_column [operation] [value];
```

The function you will be working on is:

```cpp
std::vector<std::vector<int>> project_and_filter(nlohmann::json metadata, std::string hty_file_path, std::vector<std::string> projected_columns, std::string filtered_column, int op, int value);
```

The `op` and `value` are specified as same as in Task #4. All the columns in the `projected_columns` and `filtered_column` must be in the same column group.

## Task #7 - Modify the `.hty` file (15 points)
Finally, you need to write a function to add rows to/from the `.hty` file. This includes modifying the metadata. **More importantly, you may need to rewrite the whole file.**

Adding a row is similar to the following SQL statement.

```sql
INSERT INTO hty_file_path (column_name_1, column_name_2, ...)
VALUES 
  (row_1_value_1, row_1_value_2, ...), 
  (row_2_value_1, row_2_value_2, ...),
  ...;
```

The function you will be working on is:

```cpp
void add_row(nlohmann::json metadata, std::string hty_file_path, std::string modified_hty_file_path, std::vector<std::vector<int>> rows);
```

The rewritten file will be at `modified_hty_file_path`.

## Code Style
You should follow a good coding convention. In this class, please stick with the *CMU 15-213's Code Style*.

https://www.cs.cmu.edu/afs/cs/academic/class/15213-f24/www/codeStyle.html

## FAQ
Q: I cannot use `Makefile`.

A: Please make sure that there is `bin/` directory or not.

## Grading
- 100% from all the tasks
- Score penalties can be given due to the code style

