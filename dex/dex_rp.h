/*@
XOC Release License

Copyright (c) 2013-2014, Alibaba Group, All rights reserved.

    compiler@aliexpress.com

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Su Zhenyu nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

author: Su Zhenyu
@*/
#ifndef _DEX_RP_H_
#define _DEX_RP_H_

class DEX_RP : public IR_RP {
    bool m_has_insert_stuff;
public:
    DEX_RP(Region * ru, IR_GVN * gvn) : IR_RP(ru, gvn)
    { m_has_insert_stuff = false; }
    virtual ~DEX_RP() {}

    /*
    virtual bool is_promotable(IR const* ir) const
    {
        if (ir->is_array()) {
            IR * sub = ARR_sub_list(ir);
            ASSERT0(sub);
            if (cnt_list(sub) == 1) {
                if (sub->is_pr() && PR_no(sub) == 2) {
                    return false;
                }
            }
        }
        return IR_RP::is_promotable(ir);
    }

    void insert_stuff_code(IR const* ref, Region * ru, IR_GVN * gvn)
    {
        ASSERT0(ref->is_array());

        IR * stmt = ref->get_stmt();
        ASSERT0(stmt);
        IRBB * stmt_bb = stmt->get_bb();
        ASSERT0(stmt_bb);

        IR_DU_MGR * dumgr = ru->get_du_mgr();

        C<IR*> * ct = NULL;
        BB_irlist(stmt_bb).find(stmt, &ct);
        ASSERT0(ct != NULL);

        //Insert stuff code as you need. It will slow down the benchmark.
        UINT num_want_to_insert = 30;
        for (UINT i = 0; i < num_want_to_insert; i++) {
            IR * newref = ru->dupIRTree(ref);
            dumgr->copyIRTreeDU(newref, ref, true);
            IR * stpr = ru->buildStorePR(ru->buildPrno(newref->get_type()),
                                            newref->get_type(), newref);
            ru->allocRefForPR(stpr);
            IR_may_throw(stpr) = true;

            //New IR has same VN with original one.
            gvn->set_mapIR2VN(stpr, gvn->mapIR2VN(ref));

            BB_irlist(stmt_bb).insert_before(stpr, ct);
        }
    }

    virtual void handleAccessInBody(IR * ref, IR * delegate, IR * delegate_pr,
                                    TMap<IR*, SList<IR*>*> &
                                            delegate2has_outside_uses_ir_list,
                                    TTab<IR*> & restore2mem,
                                    List<IR*> & fixup_list,
                                    TMap<IR*, IR*> & delegate2stpr,
                                    LI<IRBB> const* li,
                                    IRIter & ii)
    {
        if (!m_has_insert_stuff &&
            ref->is_array() &&
            (m_ru->isRegionName(
                "Lsoftweg/hw/performance/CPUTest;::arrayElementsDouble") ||
             m_ru->isRegionName(
                 "Lsoftweg/hw/performance/CPUTest;::arrayElementsSingle"))) {
            m_has_insert_stuff = true;
            insert_stuff_code(ref, m_ru, m_gvn);
        }
        IR_RP::handleAccessInBody(ref, delegate, delegate_pr,
                                delegate2has_outside_uses_ir_list,
                                restore2mem, fixup_list,
                                delegate2stpr, li, ii);
    }
    */
};

#endif
