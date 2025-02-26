Las app requires C++20 to work. To ensure smooth use, app is run inside container.
After downloading Las app code please carry out following steps:

1. Install podman tool for managing containers on your system if not installed.
2. Run "las_setup.sh" script from directory containing downloaded las code to install las in your system. This will create special podman image called "las_app_image" dedicated for las app. If for some reason "las_app_image" will be removed you can run "las_setup.sh" script from your "~/.las" directory to create that image once again.
3. If above steps was performed successfully you can use "$HOME/.las/las.sh" or better try to add in your "~/.bashrc" entry for "las" into PATH variable to be able to use las app throughout the system:

    PATH=$PATH:$HOME/.las
    alias las="las.sh"
4. Activate the new entry for las:
    source ~/.bashrc
5. Run las app, which executes its commands inside container:
    e.g. las diff head file1 file2
