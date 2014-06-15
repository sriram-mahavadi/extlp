git filter-branch --commit-filter '
        if [ "$GIT_COMMITTER_NAME" = "Sri Harsha Nooli" ];
        then
                GIT_COMMITTER_NAME="Venkata Sai Sriram Mahavadi";
                GIT_AUTHOR_NAME="Sriram Mahavadi";
                GIT_COMMITTER_EMAIL="sriram.mahavadi@gmail.com";
                GIT_AUTHOR_EMAIL="sriram.mahavadi@gmail.com";
                git commit-tree "$@";
        else
                git commit-tree "$@";
        fi' HEAD
