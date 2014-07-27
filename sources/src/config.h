/***************************************************************************
 * This file is part of reportabug                                         *
 *                                                                         *
 * This library is free software; you can redistribute it and/or           *
 * modify it under the terms of the GNU Lesser General Public              *
 * License as published by the Free Software Foundation; either            *
 * version 3.0 of the License, or (at your option) any later version.      *
 *                                                                         *
 * This library is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU        *
 * Lesser General Public License for more details.                         *
 *                                                                         *
 * You should have received a copy of the GNU Lesser General Public        *
 * License along with this library.                                        *
 ***************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H


/*
 * configuration of creating an issue using GitHub API
 */
// enable this function
#define ENABLE_GITHUB true
// combobox text
#define GITHUB_COMBOBOX "I want to report a bug using my GitHub account"
// the owner of the source repository
#define OWNER "arcan1s"
// project name
#define PROJECT "reportabug"
// issues url; in the most cases do not touch it
// available tags are $PROJECT, $OWNER
#define ISSUES_URL "https://api.github.com/repos/$OWNER/$PROJECT/issues"
// tags defaults
#define TAG_TITLE "A new bug"
#define TAG_BODY "Some error occurs"
// the following tags will work only if user has push access
// if they will be empty, they will be ignored
#define TAG_ASSIGNEE "$OWNER"
#define TAG_MILESTONE ""
// comma separated
#define TAG_LABELS "auto"

/*
 * configuration of creating an issue using GitReports
 *
 * please, visit the following site: https://gitreports.com/
 * and set up it for your repository
 */
// enable this function
#define ENABLE_GITREPORT true
// combobox text
#define GITREPORT_COMBOBOX "GitHub? I don't understand what do you want from me!"
// public link; in the most cases do not touch it
// available tags are $PROJECT, $OWNER
#define PUBLIC_URL "https://gitreports.com/issue/$OWNER/$PROJECT"


#endif /* CONFIG_H */
