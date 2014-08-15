/* This file is part of VoltDB.
 * Copyright (C) 2008-2014 VoltDB Inc.
 *
 * This file contains original code and/or modifications of original code.
 * Any modifications made by VoltDB Inc. are licensed under the following
 * terms and conditions:
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with VoltDB.  If not, see <http://www.gnu.org/licenses/>.
 */
/* Copyright (C) 2008 by H-Store Project
 * Brown University
 * Massachusetts Institute of Technology
 * Yale University
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef HSTORESEQSCANNODE_H
#define HSTORESEQSCANNODE_H

#include "common/common.h"
#include "abstractscannode.h"

namespace voltdb {

class AbstractExpression;

/**
 *
 */
class SeqScanPlanNode : public AbstractScanPlanNode {
   public:
        SeqScanPlanNode(CatalogId id) : AbstractScanPlanNode(id), m_isSemiScan(false) {
            // Do nothing
        }
        SeqScanPlanNode() : AbstractScanPlanNode(), m_isSemiScan(false) {
            // Do nothing
        }

        /*
         * If the output table needs to be cleared then this SeqScanNode is for an executor that created
         * its own output table rather then forwarding a reference to the persistent table being scanned.
         * It still isn't necessarily safe to delete the output table since an inline projection node/executor
         * may have created the table (and will also delete it) so check if there is an inline projection node.
         *
         * This is a fragile approach to determining whether or not to delete the output table. Maybe
         * it is safer to have the inline nodes be deleted first and set the output table of the
         * enclosing plannode to NULL so the delete can be safely repeated.
         */
        ~SeqScanPlanNode();

        virtual PlanNodeType getPlanNodeType() const { return (PLAN_NODE_TYPE_SEQSCAN); }

        std::string debugInfo(const std::string &spacer) const;

        void setSemiScanFlag(bool isSemiScan) {
            m_isSemiScan = isSemiScan;
            // if it is a semi-scan - IN (SELECT ...) - then it's a subquery
            if (m_isSemiScan) {
                m_isSubQuery = true;
            }
        }

        bool isSemiScan() const {
            return m_isSemiScan;
        }

    private:
        // If the indicator is set to true, the scan finishes on the first predicate hit
        bool m_isSemiScan;
};

}

#endif
