package com.cloudnotes.service.service.impl;

import com.cloudnotes.service.common.Result;
import com.cloudnotes.service.entity.Note;
import com.cloudnotes.service.entity.ShareRelation;
import com.cloudnotes.service.mapper.ShareNoteMapper;
import com.cloudnotes.service.service.ShareNoteService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

@Service
public class ShareNoteServiceImpl implements ShareNoteService {
    @Autowired
    ShareNoteMapper shareNoteMapper;
    @Override
    public Result delete(Integer id) {
        shareNoteMapper.deleteById(id);
        return new Result("0","删除成功",null);
    }
    @Override
    public List<ShareRelation> find(Integer sid) {
        return shareNoteMapper.findShareRelationsByShareId(sid);
    }
}
