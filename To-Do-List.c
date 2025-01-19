#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 100
#define MAX_DESCRIPTION 1000

typedef struct {
  int isDone;
  char description[MAX_DESCRIPTION];
  int priority;
} Task;

void addTask(Task task[], int *taskCount) {
    if (*taskCount >= MAX_TASKS) {
        printf("Maximum number of tasks reached.\n");
        return;
     }
    printf("Enter task description: ");
    getchar();
    fgets(task[*taskCount].description, MAX_DESCRIPTION, stdin);
  
    printf("Enter task priority (1=High, 2=Medium, 3=Low): ");
    int priority;
    
    do {
        scanf("%d", &priority);
        if (priority < 1 || priority > 3) {
            printf("Invalid input. Please enter 1, 2, or 3: ");
        }
    } while (priority < 1 || priority > 3);

    task[*taskCount].priority = priority;
    task[*taskCount].isDone = 0;

    (*taskCount)++;
    printf("Task added successfully!\n");
}

void viewTasks(Task tasks[], int taskCount) {
    printf("Your Tasks: \n");
    for (int i = 0; i < taskCount; i++) {
        printf("%d. %s (Priority: %d) - %s", i + 1, 
        tasks[i].isDone ? "X" : " ",    
        tasks[i].priority, 
        tasks[i].description);
  }
}

void markTaskComplete(Task tasks[], int taskCount) {
    int taskNumber;
    printf("Enter task number to mark as complete: ");
    scanf("%d", &taskNumber);

    if (taskNumber > 0 && taskNumber <= taskCount) {
        tasks[taskNumber - 1].isDone = 1;
        printf("Task marked as complete!\n");
    } 
    else {
        printf("Invalid task number.\n");
    }
}

void deleteTask(Task tasks[], int *taskCount) {
    int taskNumber;
    printf("Enter task number to delete: ");
    scanf("%d", &taskNumber);

    if (taskNumber > 0 && taskNumber <= *taskCount) {
        for (int i = taskNumber - 1; i < *taskCount - 1; i++) {
            tasks[i] = tasks[i + 1];
      }
      (*taskCount)--;
      printf("Task deleted successfully!\n");
    } 
    else {
        printf("Invalid task number.\n");
    }   
}

void saveTasksToFile(Task tasks[], int taskCount, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for saving.\n");
        return;
    }

    for (int i = 0; i < taskCount; i++) {
        fprintf(file, "%s,%d,%d", tasks[i].description, tasks[i].isDone,
        tasks[i].priority);
        
        if (i < taskCount - 1) {
            fprintf(file, "\n");
        }
    }
    fclose(file);
    printf("Tasks saved to %s.\n", filename);
}

void loadTasksFromFile(Task tasks[], int *taskCount, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No existing task file found. Starting fresh.\n");
        return;
    }

    char line[150];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        char *token = strtok(line, ",");
        strncpy(tasks[*taskCount].description, token, MAX_DESCRIPTION);

        token = strtok(NULL, ",");
        tasks[*taskCount].isDone = atoi(token);

        token = strtok(NULL, ",");
        tasks[*taskCount].priority = atoi(token);

        (*taskCount)++;
    }

    fclose(file);
    printf("Tasks loaded from %s.\n", filename);
}

int main(void) {
    Task tasks[MAX_TASKS];
    int taskCount = 0;
    int choice;

    loadTasksFromFile(tasks, &taskCount, "tasks.csv");


    while (1) {
        printf("\nTo-Do List\n");
        printf("1. Add Task\n");
        printf("2. View Tasks\n");
        printf("3. Mark Task as Complete\n");
        printf("4. Delete Task\n");
        printf("5. Save and Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addTask(tasks, &taskCount);
                break;
            case 2:
                viewTasks(tasks, taskCount);
                break;
            case 3:
                markTaskComplete(tasks, taskCount);
                break;
            case 4:
                deleteTask(tasks, &taskCount);
                break;
            case 5:
                saveTasksToFile(tasks, taskCount, "tasks.csv");
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}
