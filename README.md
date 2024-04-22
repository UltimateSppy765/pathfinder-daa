# PathFinder
This is a GUI interface for the solution to my algorithms assignment, requiring to find the path from a source point to a destination point in a matrix using Backtracking and Branch & Bound techniques. The interface was made using Qt and compiled with Microsoft Visual C++ compilers. Logic for the path finding can be found in [`utilities.cpp`](PathFinder/utilities.cpp).

## Usage
Download the app from the releases section in the repository. Once downloaded, open `PathFinder.exe` in the `App` folder after unzipping.
> [!WARNING]
> Do not delete the additional files and folders inside the `App` folder, they're needed by the app in order to run.

The initial interface should look like this:

![image](https://github.com/UltimateSppy765/pathfinder-daa/assets/73820738/783607fc-2e8d-4105-abb6-6c6d5f884be0)

You can select a cell to mark it as visitable (green), or select it again to mark it as non-visitable (grey).<br>
*(Drag clicking allows to toggle multiple cells at once.)*

![image](https://github.com/UltimateSppy765/pathfinder-daa/assets/73820738/291e6d52-01e6-47b4-a48b-7620ff6e79de)

Use the Options pane on the right to mark a cell as source/destination.

![image](https://github.com/UltimateSppy765/pathfinder-daa/assets/73820738/bd660c95-2eb6-4856-a088-5bdb16099850)

Use the combo box to select the algorithm for finding the path.

![image](https://github.com/UltimateSppy765/pathfinder-daa/assets/73820738/bfad57f4-ec69-4972-913c-1aea47b67d37)

Click `Find Path` to run the algorithm and get the results.

![image](https://github.com/UltimateSppy765/pathfinder-daa/assets/73820738/1f372607-ab47-480a-be8c-5ba02caad5c4)
