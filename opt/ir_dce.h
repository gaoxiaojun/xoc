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
#ifndef _IR_DCE_H_
#define _IR_DCE_H_

class EFFECT_STMT : public BITSET {
public:
	void bunion(INT elem)
	{
		BITSET::bunion(elem);
	}
};

//Perform dead code and redundant control flow elimination.
class IR_DCE : public IR_OPT {
protected:
	MD_SYS * m_md_sys;
	DT_MGR * m_dm;
	REGION * m_ru;
	IR_CFG * m_cfg;
	CDG * m_cdg;
	IR_DU_MGR * m_du;
	CIR_ITER m_citer;
	bool m_is_elim_cfs; //Eliminate control flow structure.
	bool m_is_ssa_available; //indicate if SSA info is available.

	void fix_control_flow(LIST<IR_BB*> & bblst, LIST<C<IR_BB*>*> & ctlst);
	bool find_effect_kid(IN IR_BB * bb, IN IR * ir,
						 IN EFFECT_STMT & is_stmt_effect);
	bool preserve_cd(IN OUT BITSET & is_bb_effect,
					 IN OUT EFFECT_STMT & is_stmt_effect,
					 IN OUT LIST<IR const*> & act_ir_lst);
	void mark_effect_ir(IN OUT EFFECT_STMT & is_stmt_effect,
						IN OUT BITSET & is_bb_effect,
						IN OUT LIST<IR const*> & work_list);

	bool is_effect_write(VAR * v) const
	{ return VAR_is_global(v) || VAR_is_volatile(v); }

	bool is_effect_read(VAR * v) const
	{ return VAR_is_volatile(v); }

	bool is_cfs(IR const* ir) const
	{
		switch (IR_type(ir)) {
		case IR_TRUEBR:
		case IR_FALSEBR:
		case IR_GOTO:
		case IR_IGOTO:
			return true;
		default: IS_TRUE0(ir->is_stmt_in_bb());
		}
		return false;
	}

	bool is_ssa_available() const { return m_is_ssa_available; }

	void iter_collect(IN OUT EFFECT_STMT & is_stmt_effect,
					  IN OUT BITSET & is_bb_effect,
					  IN OUT LIST<IR const*> & work_list);
	void record_all_ir(IN OUT SVECTOR<SVECTOR<IR*>*> & all_ir);
	void revise_successor(IR_BB * bb, C<IR_BB*> * bbct, IR_BB_LIST * bbl);

	bool check_stmt(IR const* ir);
	bool check_call(IR const* ir);
public:
	IR_DCE(REGION * ru)
	{
		IS_TRUE0(ru != NULL);
		m_ru = ru;
		m_dm = ru->get_dm();
		m_cfg = ru->get_cfg();
		m_du = ru->get_du_mgr();
		m_md_sys = ru->get_md_sys();
		m_is_elim_cfs = false;
		m_is_ssa_available = false;
		m_cdg = NULL;
	}
	virtual ~IR_DCE() {}

	void dump(IN EFFECT_STMT const& is_stmt_effect,
			  IN BITSET const& is_bb_effect,
			  IN SVECTOR<SVECTOR<IR*>*> & all_ir);

	virtual CHAR const* get_opt_name() const { return "Dead Code Eliminiation"; }

	OPT_TYPE get_opt_type() const { return OPT_DCE; }

	void set_elim_cfs(bool doit) { m_is_elim_cfs = doit; }

	void set_ssa_available(bool avail) { m_is_ssa_available = avail; }

	virtual bool perform(OPT_CTX & oc);
};
#endif
