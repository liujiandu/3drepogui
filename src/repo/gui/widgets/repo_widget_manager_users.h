/**
 *  Copyright (C) 2015 3D Repo Ltd
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

//------------------------------------------------------------------------------
// GUI
#include "repo_widget_tree_editable.h"
#include "repo_widget_tree_clickable.h"
#include "../dialogs/repo_dialog_user.h"
#include "../primitives/repo_idbcache.h"
#include "../../workers/repo_worker_users.h"
#include "../primitives/repo_standard_item.h"

//------------------------------------------------------------------------------
// CORE
#include <repo/core/model/bson/repo_bson_user.h>

//------------------------------------------------------------------------------
// Qt
#include <QDialog>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QThreadPool>
#include <QItemSelection>
#include <QMessageBox>
#include <QMutex>

Q_DECLARE_METATYPE(repo::core::model::RepoUser)

namespace repo {
namespace gui {
namespace widget {

	class UsersManagerWidget : public EditableTreeWidget
	{
		Q_OBJECT

			static const QString COLUMNS_SETTINGS;

		enum class Columns { USERNAME, FIRST_NAME, LAST_NAME, EMAIL, ROLES, API_KEYS };

	public:

		explicit UsersManagerWidget(QWidget *parent = 0);

		~UsersManagerWidget();

		public slots:

		//! Adds a fresh list of custom roles.
		void addCustomRoles(const std::list<std::string> &);

		void addItem()
		{
			showEditDialog();
		}

		//! Adds user to the list of users.
		void addUser(const repo::core::model::RepoUser &user);

		//! Updates selected user.
		void edit();

		//! Updates user based on model index.
		void edit(const QModelIndex &index);

		//! Returns a currently selected user if any.
		repo::core::model::RepoUser getUser() const;

		//! Returns a user specified by the model index.
		repo::core::model::RepoUser getUser(const QModelIndex &index) const;

		virtual void refresh()
		{
			refresh(repo::core::model::RepoUser(),
				repo::worker::UsersWorker::Command::INSERT);
		}

		void refresh(
			const repo::core::model::RepoUser& user,
			const repo::worker::UsersWorker::Command& command);

		void copyItem()
		{
			showEditDialog(getUser(), EditableTreeWidget::Action::COPY);
		}

		//! Drops user from the database.
		void removeItem();

		//! Shows the user dialog and saves edits to the database.
		void showEditDialog()
		{
			showEditDialog(repo::core::model::RepoUser(), EditableTreeWidget::Action::ADD);
		}

		//! Shows the user dialog and saves edits to the database.
		void showEditDialog(
			const repo::core::model::RepoUser &user,
			EditableTreeWidget::Action action);

		void setDatabasesWithProjects(const std::map<std::string, std::list<std::string> > &rdwp);

	public:

		void setDBConnection(
			repo::RepoController *controller,
			const repo::RepoController::RepoToken *token,
			const std::string &database);

	private:

		//! List of custom roles updated upon each refresh.
		std::list<std::string> customRolesList;

		//! Mapping of databases to their associated projects.
		std::map<std::string, std::list<std::string> > databasesWithProjects;

		const repo::RepoController::RepoToken* token;

		std::string database;

		repo::RepoController *controller;
	};

} // widgets
}
} // repo

