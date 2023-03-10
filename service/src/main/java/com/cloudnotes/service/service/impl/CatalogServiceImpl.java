package com.cloudnotes.service.service.impl;

import com.cloudnotes.service.common.Result;
import com.cloudnotes.service.entity.Catalogue;
import com.cloudnotes.service.entity.Note;
import com.cloudnotes.service.mapper.CatalogMapper;
import com.cloudnotes.service.mapper.NoteMapper;
import com.cloudnotes.service.service.CatalogService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class CatalogServiceImpl implements CatalogService {
    @Autowired
    CatalogMapper catalogMapper;
    @Autowired
    NoteMapper noteMapper;
    @Override
    public List<Catalogue> find(Integer userId) {
        return catalogMapper.findByUserId(userId);
    }
    @Override
    public Result update(Catalogue catalogue) {
        catalogMapper.save(catalogue);
        return new Result("0","修改成功",null);
    }

    @Override
    public Result create(Catalogue catalogue) {
        Catalogue temp=catalogMapper.save(catalogue);
        Integer id=temp.getId();
        return new Result("0","修改成功",id);
    }

    @Override
    public Result delete(Integer id) {
        Catalogue temp=catalogMapper.findCatalogueById(id);
        catalogMapper.deleteById(id);
        List<Note> notes=noteMapper.findNotesByCatalogId(id);
        for(Note note:notes){
            note.setCatalogId(temp.getFatherId());
            noteMapper.save(note);
        }
        return new Result("0","删除成功",null);
    }

}
