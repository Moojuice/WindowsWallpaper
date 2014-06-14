wallpaperapp.cpp v1.0

Last updated 6/5/2014

INSTRUCTIONS:
1. Fill out the settings.txt file provided! And make sure it is located in the same folder as the .exe! Reminder: ALL URLS MUST USE DOUBLE BACKSLASHES.
This means it should look like C:\\Users\\Bob\\Pictures\\. Another reminder: YOU MUST CONCLUDE THE URLS WITH DOUBLE BACKSLASHES

2. Time to shuffle is set in hours, I have it set to the value for 1 second at the moment to prove fractions work. The sky's the limit for this,
where the sky is your computer's refresh rate and/or the smallest size doubles can can. 

3. 1 means yes, 0 means no 

4. If you set DayNight to 0, then you do NOT need a day folder and night folder. If you set it to 1, you need to create two folders, one for day
wallpapers and one for night wallpapers. They cannot be empty, so have fun and fill them up!  

5. Tip for getting URLs, click on the url line with the folder icon on the far left to turn it into a URL. Copy and paste into a word processor
and replace all \'s with \\'s. Success! 

PURPOSE: Save your eyes and customize how your wallpaper looks! Do you hate it when you are working late at night and then your wallpaper shifts
to a bright color? You can change that now! 
The program is used for controlled desktop wallpaper functionality. Allows the user more direct control over how the wallpaper shuffles. This also acts as a 
simple framework for users new to C++ to use if they want to start manipulating wallpapers. I spent pretty much the entirity of my time just trying
to find a way to get the wallpapers to display properly, so now that it is finished, it opens a whole new range of opportunities to play with.
Features include designating whatever time between shuffles as well as a day/night feature which allows the user to include two folders, "Day" and
"Night" which the program uses to sort through folders when the computer locally reports it is in the daytime or nighttime. Seasonal folders possible
improvement not included in this version.
Another idea to possibly expand on is to auto-assign pictures to a day or night category based on the average pixel darkness of the pictures. 

SKILLS USED: Introduction to WindowsAPI, broad usage of searching for references to learn from and use as examples (time, desktop API, threading
rand), multithreading in C++

RETURN VALUES: SUCCESS, BAD_READ, USAGE, EMPTY_FOLDER

COMMENTS: As mentioned earlier, I would like to implement more features such as: processing all of the images in a folder to get the avg RGB value,
based on this # determine if the picture should be day or night; adding seasonal folders so you never need to see snow in the summer.
