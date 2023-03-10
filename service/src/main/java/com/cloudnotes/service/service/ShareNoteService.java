package com.cloudnotes.service.service;

import com.cloudnotes.service.common.Result;
import com.cloudnotes.service.entity.ShareRelation;

import java.util.List;

public interface ShareNoteService {
    public Result delete(Integer id);
    public List<ShareRelation> find(Integer sid);
}
